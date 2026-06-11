#include "gpio_chardev.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int signo) {
    (void)signo;
    keep_running = 0;
}

static void usage(const char *program) {
    fprintf(stderr,
            "Usage: %s <chip> <line> <active_level> [poll_ms] [max_events]\n"
            "\n"
            "Example:\n"
            "  %s gpiochip0 13 1\n"
            "  %s /dev/gpiochip0 13 0 100 20\n",
            program, program, program);
}

static void print_state(int value, int active_level) {
    time_t now = time(NULL);
    struct tm local_time;
    char stamp[32];

    if (localtime_r(&now, &local_time) == NULL) {
        snprintf(stamp, sizeof(stamp), "unknown-time");
    } else {
        strftime(stamp, sizeof(stamp), "%H:%M:%S", &local_time);
    }

    printf("%s raw=%d state=%s\n", stamp, value,
           value == active_level ? "pressed" : "released");
    fflush(stdout);
}

int main(int argc, char **argv) {
    const char *chip;
    unsigned int line;
    int active_level;
    unsigned long poll_ms = 100;
    unsigned long max_events = 0;
    unsigned long events = 0;
    int last_value = -1;
    struct gpio_line button;

    if (argc < 4 || argc > 6) {
        usage(argv[0]);
        return 2;
    }

    chip = argv[1];
    if (parse_uint_arg(argv[2], &line) < 0 ||
        parse_level_arg(argv[3], &active_level) < 0) {
        usage(argv[0]);
        return 2;
    }

    if (argc >= 5 && parse_ulong_arg(argv[4], &poll_ms) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (argc >= 6 && parse_ulong_arg(argv[5], &max_events) < 0) {
        usage(argv[0]);
        return 2;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (gpio_open_input(&button, chip, line, "ruyi-button-read") < 0) {
        fprintf(stderr, "open GPIO input failed: %s\n", strerror(errno));
        return 1;
    }

    printf("Button read started: chip=%s line=%u active=%d poll=%lums max_events=%lu\n",
           chip, line, active_level, poll_ms, max_events);
    fflush(stdout);

    while (keep_running && (max_events == 0 || events < max_events)) {
        int value;

        if (gpio_get_value(&button, &value) < 0) {
            fprintf(stderr, "read GPIO input failed: %s\n", strerror(errno));
            gpio_close(&button);
            return 1;
        }

        if (value != last_value) {
            print_state(value, active_level);
            last_value = value;
            events++;
        }

        sleep_ms(poll_ms);
    }

    gpio_close(&button);
    printf("Button read stopped after %lu event(s).\n", events);
    return 0;
}

