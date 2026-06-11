#include "gpio_chardev.h"

#ifdef __linux__
#include <errno.h>
#include <fcntl.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

static int open_chip(const char *chip) {
    char path[128];

    if (chip == NULL || chip[0] == '\0') {
        errno = EINVAL;
        return -1;
    }

    if (chip[0] == '/') {
        return open(chip, O_RDONLY | O_CLOEXEC);
    }

    if (snprintf(path, sizeof(path), "/dev/%s", chip) >= (int)sizeof(path)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    return open(path, O_RDONLY | O_CLOEXEC);
}

static void init_line(struct gpio_line *gpio, unsigned int line) {
    memset(gpio, 0, sizeof(*gpio));
    gpio->chip_fd = -1;
    gpio->line_fd = -1;
    gpio->line = line;
}

int gpio_open_output(struct gpio_line *gpio, const char *chip, unsigned int line,
                     int initial_value, const char *consumer) {
    struct gpiohandle_request request;

    if (gpio == NULL) {
        errno = EINVAL;
        return -1;
    }

    init_line(gpio, line);
    gpio->chip_fd = open_chip(chip);
    if (gpio->chip_fd < 0) {
        return -1;
    }

    memset(&request, 0, sizeof(request));
    request.lineoffsets[0] = line;
    request.flags = GPIOHANDLE_REQUEST_OUTPUT;
    request.lines = 1;
    request.default_values[0] = initial_value ? 1 : 0;
    snprintf(request.consumer_label, sizeof(request.consumer_label), "%s",
             consumer != NULL ? consumer : "ruyi-gpio-output");

    if (ioctl(gpio->chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &request) < 0) {
        gpio_close(gpio);
        return -1;
    }

    gpio->line_fd = request.fd;
    return 0;
}

int gpio_open_input(struct gpio_line *gpio, const char *chip, unsigned int line,
                    const char *consumer) {
    struct gpiohandle_request request;

    if (gpio == NULL) {
        errno = EINVAL;
        return -1;
    }

    init_line(gpio, line);
    gpio->chip_fd = open_chip(chip);
    if (gpio->chip_fd < 0) {
        return -1;
    }

    memset(&request, 0, sizeof(request));
    request.lineoffsets[0] = line;
    request.flags = GPIOHANDLE_REQUEST_INPUT;
    request.lines = 1;
    snprintf(request.consumer_label, sizeof(request.consumer_label), "%s",
             consumer != NULL ? consumer : "ruyi-gpio-input");

    if (ioctl(gpio->chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &request) < 0) {
        gpio_close(gpio);
        return -1;
    }

    gpio->line_fd = request.fd;
    return 0;
}

int gpio_set_value(const struct gpio_line *gpio, int value) {
    struct gpiohandle_data data;

    if (gpio == NULL || gpio->line_fd < 0) {
        errno = EINVAL;
        return -1;
    }

    memset(&data, 0, sizeof(data));
    data.values[0] = value ? 1 : 0;

    return ioctl(gpio->line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

int gpio_get_value(const struct gpio_line *gpio, int *value) {
    struct gpiohandle_data data;

    if (gpio == NULL || gpio->line_fd < 0 || value == NULL) {
        errno = EINVAL;
        return -1;
    }

    memset(&data, 0, sizeof(data));
    if (ioctl(gpio->line_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0) {
        return -1;
    }

    *value = data.values[0] ? 1 : 0;
    return 0;
}

void gpio_close(struct gpio_line *gpio) {
    if (gpio == NULL) {
        return;
    }

    if (gpio->line_fd >= 0) {
        close(gpio->line_fd);
        gpio->line_fd = -1;
    }

    if (gpio->chip_fd >= 0) {
        close(gpio->chip_fd);
        gpio->chip_fd = -1;
    }
}

int parse_uint_arg(const char *text, unsigned int *value) {
    char *end = NULL;
    unsigned long parsed;

    if (text == NULL || value == NULL || text[0] == '\0') {
        return -1;
    }

    errno = 0;
    parsed = strtoul(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed > 1024UL * 1024UL) {
        return -1;
    }

    *value = (unsigned int)parsed;
    return 0;
}

int parse_level_arg(const char *text, int *value) {
    unsigned int parsed;

    if (parse_uint_arg(text, &parsed) < 0 || parsed > 1U) {
        return -1;
    }

    *value = (int)parsed;
    return 0;
}

int parse_ulong_arg(const char *text, unsigned long *value) {
    char *end = NULL;
    unsigned long parsed;

    if (text == NULL || value == NULL || text[0] == '\0') {
        return -1;
    }

    errno = 0;
    parsed = strtoul(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0') {
        return -1;
    }

    *value = parsed;
    return 0;
}

void sleep_ms(unsigned long ms) {
    struct timespec delay;

    delay.tv_sec = (time_t)(ms / 1000UL);
    delay.tv_nsec = (long)((ms % 1000UL) * 1000000UL);

    while (nanosleep(&delay, &delay) < 0 && errno == EINTR) {
    }
}

#else
#error "This project must be built on Linux because it uses linux/gpio.h."
#endif
