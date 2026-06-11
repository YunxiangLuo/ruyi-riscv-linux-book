CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude
LDFLAGS ?=

BUILD_DIR := build
BUILD_STAMP := $(BUILD_DIR)/.dir
COMMON_SRC := src/gpio_chardev.c
COMMON_OBJ := $(BUILD_DIR)/gpio_chardev.o
TARGETS := $(BUILD_DIR)/gpio_state_demo

.PHONY: build run clean

build: $(TARGETS)

$(BUILD_STAMP):
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_STAMP)

$(COMMON_OBJ): $(COMMON_SRC) include/gpio_chardev.h | $(BUILD_STAMP)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(COMMON_SRC) -o $@

$(BUILD_DIR)/gpio_state_demo: src/gpio_state_demo.c $(COMMON_OBJ) include/gpio_chardev.h | $(BUILD_STAMP)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/gpio_state_demo.c $(COMMON_OBJ) $(LDFLAGS) -o $@

run: $(BUILD_DIR)/gpio_state_demo
	@if [ -z "$$LED_CHIP" ] || [ -z "$$LED_LINE" ] || [ -z "$$LED_ACTIVE_LEVEL" ] || \
	      [ -z "$$LED_INACTIVE_LEVEL" ] || [ -z "$$BUTTON_CHIP" ] || \
	      [ -z "$$BUTTON_LINE" ] || [ -z "$$BUTTON_ACTIVE_LEVEL" ]; then \
		echo "Set LED_CHIP, LED_LINE, LED_ACTIVE_LEVEL, LED_INACTIVE_LEVEL, BUTTON_CHIP, BUTTON_LINE, BUTTON_ACTIVE_LEVEL first."; \
		exit 1; \
	fi
	$(BUILD_DIR)/gpio_state_demo "$$LED_CHIP" "$$LED_LINE" "$$LED_ACTIVE_LEVEL" "$$LED_INACTIVE_LEVEL" \
		"$$BUTTON_CHIP" "$$BUTTON_LINE" "$$BUTTON_ACTIVE_LEVEL"

clean:
	rm -rf $(BUILD_DIR)
