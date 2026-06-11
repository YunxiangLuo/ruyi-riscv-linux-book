#include "gpio_chardev.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

enum led_mode {
    MODE_OFF = 0,
    MODE_ON,
    MODE_BLINK,
};

static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int signo) {
    (void)signo;
    keep_running = 0;
}

static const char *mode_name(enum led_mode mode) {
    switch (mode) {
    case MODE_OFF:
        return "off";
    case MODE_ON:
        return "on";
    case MODE_BLINK:
        return "blink";
    default:
        return "unknown";
    }
}

static enum led_mode next_mode(enum led_mode mode) {
    return (enum led_mode)(((int)mode + 1) % 3);
}

static void usage(const char *program) {
    fprintf(stderr,
            "Usage: %s <led_chip> <led_line> <led_active> <led_inactive> "
            "<button_chip> <button_line> <button_active> "
            "[poll_ms] [debounce_ms] [max_events]\n"
            "\n"
            "Example:\n"
            "  %s gpiochip0 12 1 0 gpiochip0 13 1\n"
            "  %s /dev/gpiochip0 12 1 0 /dev/gpiochip0 13 0 50 200 10\n",
            program, program, program);
}

static int apply_mode(const struct gpio_line *led, enum led_mode mode,
                      int active_level, int inactive_level,
                      unsigned long tick, int *blink_value) {
    int value = inactive_level;

    if (mode == MODE_ON) {
        value = active_level;
    } else if (mode == MODE_BLINK) {
        if (tick % 10UL == 0) {
            *blink_value = (*blink_value == active_level) ? inactive_level : active_level;
        }
        value = *blink_value;
    } else {
        *blink_value = inactive_level;
    }

    return gpio_set_value(led, value);
}

int main(int argc, char **argv) {
    const char *led_chip;
    const char *button_chip;
    unsigned int led_line;
    unsigned int button_line;
    int led_active;
    int led_inactive;
    int button_active;
    unsigned long poll_ms = 50;
    unsigned long debounce_ms = 200;
    unsigned long max_events = 0;
    unsigned long stable_ticks_needed;
    unsigned long stable_ticks = 0;
    unsigned long tick = 0;
    unsigned long events = 0;
    int last_raw = -1;
    int stable_raw = -1;
    int last_pressed = 0;
    int blink_value;
    enum led_mode mode = MODE_OFF;
    struct gpio_line led;
    struct gpio_line button;

    if (argc < 8 || argc > 11) {
        usage(argv[0]);
        return 2;
    }

    led_chip = argv[1];
    button_chip = argv[5];
    if (parse_uint_arg(argv[2], &led_line) < 0 ||
        parse_level_arg(argv[3], &led_active) < 0 ||
        parse_level_arg(argv[4], &led_inactive) < 0 ||
        parse_uint_arg(argv[6], &button_line) < 0 ||
        parse_level_arg(argv[7], &button_active) < 0) {
        usage(argv[0]);
        return 2;
    }

    if (argc >= 9 && parse_ulong_arg(argv[8], &poll_ms) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (argc >= 10 && parse_ulong_arg(argv[9], &debounce_ms) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (argc >= 11 && parse_ulong_arg(argv[10], &max_events) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (poll_ms == 0) {
        fprintf(stderr, "poll_ms must be greater than 0.\n");
        return 2;
    }

    stable_ticks_needed = debounce_ms / poll_ms;
    if (stable_ticks_needed == 0 || debounce_ms % poll_ms != 0) {
        stable_ticks_needed++;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (gpio_open_output(&led, led_chip, led_line, led_inactive,
                         "ruyi-state-led") < 0) {
        fprintf(stderr, "open LED GPIO output failed: %s\n", strerror(errno));
        return 1;
    }

    if (gpio_open_input(&button, button_chip, button_line,
                        "ruyi-state-button") < 0) {
        fprintf(stderr, "open button GPIO input failed: %s\n", strerror(errno));
        gpio_close(&led);
        return 1;
    }

    blink_value = led_inactive;
    printf("State demo started: led=%s:%u button=%s:%u poll=%lums debounce=%lums\n",
           led_chip, led_line, button_chip, button_line, poll_ms, debounce_ms);
    printf("mode=%s\n", mode_name(mode));
    fflush(stdout);

    while (keep_running && (max_events == 0 || events < max_events)) {
        int raw;
        int pressed;

        if (gpio_get_value(&button, &raw) < 0) {
            fprintf(stderr, "read button failed: %s\n", strerror(errno));
            gpio_close(&button);
            gpio_close(&led);
            return 1;
        }

        if (raw != last_raw) {
            printf("raw button=%d\n", raw);
            last_raw = raw;
            stable_ticks = 0;
        } else if (stable_ticks < stable_ticks_needed) {
            stable_ticks++;
        }

        if (stable_ticks >= stable_ticks_needed && raw != stable_raw) {
            stable_raw = raw;
            pressed = stable_raw == button_active;
            printf("debounced button=%s\n", pressed ? "pressed" : "released");

            if (pressed && !last_pressed) {
                mode = next_mode(mode);
                blink_value = led_inactive;
                events++;
                printf("event=%lu mode=%s\n", events, mode_name(mode));
            }
            last_pressed = pressed;
            fflush(stdout);
        }

        if (apply_mode(&led, mode, led_active, led_inactive, tick,
                       &blink_value) < 0) {
            fprintf(stderr, "set LED failed: %s\n", strerror(errno));
            gpio_close(&button);
            gpio_close(&led);
            return 1;
        }

        tick++;
        sleep_ms(poll_ms);
    }

    if (gpio_set_value(&led, led_inactive) < 0) {
        fprintf(stderr, "warning: failed to restore inactive LED level: %s\n",
                strerror(errno));
    }

    gpio_close(&button);
    gpio_close(&led);
    printf("State demo stopped after %lu event(s).\n", events);
    return 0;
}
