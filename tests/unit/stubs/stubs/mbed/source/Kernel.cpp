// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../Kernel.h"

auto rtos::Kernel::impl::get_tick_count() -> uint64_t
{
	return leka::spy_kernel_tick_count;
}

namespace leka {

int64_t spy_kernel_tick_count = 29712345;

void spy_kernel_setTickCount(int64_t count)
{
	spy_kernel_tick_count = count;
}

void spy_kernel_addElapsedTimeToTickCount(std::chrono::milliseconds elapsed_time)
{
	spy_kernel_tick_count += elapsed_time.count();
}
}	// namespace leka
