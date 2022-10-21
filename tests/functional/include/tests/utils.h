// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/ThisThread.h"

#include "LogKit.h"

namespace utils::time {

using namespace leka;
using namespace std::chrono;

inline auto start = rtos::Kernel::Clock::now();
inline auto stop  = rtos::Kernel::Clock::now();
inline auto delta = [] { return static_cast<int>((stop - start).count()); };

}	// namespace utils::time

// NOLINTNEXTLINE
#define utils_start(msg)                                                                                               \
	do {                                                                                                               \
		logger::init();                                                                                                \
		log_ll("\n", 1);                                                                                               \
		log_info("Starting %s...\n", msg);                                                                             \
		rtos::ThisThread::sleep_for(1s);                                                                               \
		utils::time::start = rtos::Kernel::Clock::now();                                                               \
	} while (0)

// NOLINTNEXTLINE
#define utils_end()                                                                                                    \
	do {                                                                                                               \
		utils::time::stop = rtos::Kernel::Clock::now();                                                                \
		log_ll("\n", 1);                                                                                               \
		log_info("End of tests (%i ms total)", utils::time::delta());                                                  \
	} while (0)
