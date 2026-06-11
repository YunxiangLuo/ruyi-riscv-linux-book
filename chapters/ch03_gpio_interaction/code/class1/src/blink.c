#include "gpio_chardev.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int signo) {
    (void)signo;
    keep_running = 0;
}

static void usage(const char *program) {
    fprintf(stderr,
            "Usage: %s <chip> <line> <active_level> <inactive_level> "
            "[on_ms] [off_ms] [cycles]\n"
            "\n"
            "Example:\n"
            "  %s gpiochip0 12 1 0\n"
            "  %s /dev/gpiochip0 12 1 0 500 500 10\n",
            program, program, program);
}

int main(int argc, char **argv) {
    const char *chip;
    unsigned int line;
    int active_level;
    int inactive_level;
    unsigned long on_ms = 500;
    unsigned long off_ms = 500;
    unsigned long cycles = 0;
    unsigned long completed = 0;
    struct gpio_line led;

    if (argc < 5 || argc > 8) {
        usage(argv[0]);
        return 2;
    }

    chip = argv[1];
    if (parse_uint_arg(argv[2], &line) < 0 ||
        parse_level_arg(argv[3], &active_level) < 0 ||
        parse_level_arg(argv[4], &inactive_level) < 0) {
        usage(argv[0]);
        return 2;
    }

    if (argc >= 6 && parse_ulong_arg(argv[5], &on_ms) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (argc >= 7 && parse_ulong_arg(argv[6], &off_ms) < 0) {
        usage(argv[0]);
        return 2;
    }
    if (argc >= 8 && parse_ulong_arg(argv[7], &cycles) < 0) {
        usage(argv[0]);
        return 2;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (gpio_open_output(&led, chip, line, inactive_level, "ruyi-blink") < 0) {
        fprintf(stderr, "open GPIO output failed: %s\n", strerror(errno));
        return 1;
    }

    printf("Blink started: chip=%s line=%u active=%d inactive=%d on=%lums off=%lums cycles=%lu\n",
           chip, line, active_level, inactive_level, on_ms, off_ms, cycles);
    fflush(stdout);

    while (keep_running && (cycles == 0 || completed < cycles)) {
        if (gpio_set_value(&led, active_level) < 0) {
            fprintf(stderr, "set active level failed: %s\n", strerror(errno));
            gpio_close(&led);
            return 1;
        }
        sleep_ms(on_ms);

        if (gpio_set_value(&led, inactive_level) < 0) {
            fprintf(stderr, "set inactive level failed: %s\n", strerror(errno));
            gpio_close(&led);
            return 1;
        }
        sleep_ms(off_ms);

        completed++;
    }

    if (gpio_set_value(&led, inactive_level) < 0) {
        fprintf(stderr, "warning: failed to restore inactive level: %s\n",
                strerror(errno));
    }

    gpio_close(&led);
    printf("Blink stopped after %lu cycle(s).\n", completed);
    return 0;
}

