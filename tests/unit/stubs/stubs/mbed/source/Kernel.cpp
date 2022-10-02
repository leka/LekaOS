// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../Kernel.h"

uint64_t rtos::Kernel::impl::get_tick_count()
{
	return leka::spy_kernel_tick_count;
}

namespace leka {

int64_t spy_kernel_tick_count = 29712345;

void spy_kernel_setTickCount(int64_t count)
{
	spy_kernel_tick_count = count;
}

}	// namespace leka
