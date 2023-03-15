// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../HighResClock.h"

ticker_data_t us_data = {};

auto get_us_ticker_data(void) -> const ticker_data_t *
{
	return &us_data;
}
