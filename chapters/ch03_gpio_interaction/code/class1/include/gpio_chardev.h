#ifndef GPIO_CHARDEV_H
#define GPIO_CHARDEV_H

#include <stddef.h>

struct gpio_line {
    int chip_fd;
    int line_fd;
    unsigned int line;
};

int gpio_open_output(struct gpio_line *gpio, const char *chip, unsigned int line,
                     int initial_value, const char *consumer);
int gpio_open_input(struct gpio_line *gpio, const char *chip, unsigned int line,
                    const char *consumer);
int gpio_set_value(const struct gpio_line *gpio, int value);
int gpio_get_value(const struct gpio_line *gpio, int *value);
void gpio_close(struct gpio_line *gpio);

int parse_uint_arg(const char *text, unsigned int *value);
int parse_level_arg(const char *text, int *value);
int parse_ulong_arg(const char *text, unsigned long *value);
void sleep_ms(unsigned long ms);

#endif

