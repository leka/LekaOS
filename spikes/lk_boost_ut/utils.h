// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/ThisThread.h"

#include "LogKit.h"

namespace utils {

using namespace leka;
using namespace std::chrono;

namespace time {

	auto start = rtos::Kernel::Clock::now();
	auto stop  = rtos::Kernel::Clock::now();
	auto delta = static_cast<int>((stop - start).count());

}	// namespace time

inline void start(const char *msg)
{
	logger::init();

	log_ll("\n", 1);
	log_info("Starting %s...\n", msg);

	rtos::ThisThread::sleep_for(1s);

	time::start = rtos::Kernel::Clock::now();
}

inline void end()
{
	time::stop	= rtos::Kernel::Clock::now();
	time::delta = static_cast<int>((time::stop - time::start).count());

	log_ll("\n", 1);
	log_info("End of tests (%i ms total)", time::delta);
}

}	// namespace utils
