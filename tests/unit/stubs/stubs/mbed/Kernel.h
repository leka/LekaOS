// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "rtos/Kernel.h"

namespace leka {

extern int64_t spy_kernel_tick_count;

void spy_kernel_setTickCount(int64_t count);

}	// namespace leka
