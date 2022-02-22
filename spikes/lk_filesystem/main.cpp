// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	while (true) {
		rtos::ThisThread::sleep_for(10s);
	}
}
