// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "hal/gpio_api.h"

void gpio_init_out(gpio_t *gpio, PinName pin)
{
	return;
}

void gpio_write(gpio_t *gpio, int value)
{
	return;
}

auto gpio_read(gpio_t *obj) -> int
{
	return 0;
}

auto gpio_is_connected(const gpio_t *obj) -> int
{
	return 1;
}
