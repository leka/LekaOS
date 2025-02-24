// Leka - LekaOS
// Copyright APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>

#include "rtos/ThisThread.h"

using namespace std::chrono;

auto main() -> int
{
	while (true) {
		auto now = rtos::Kernel::Clock::now().time_since_epoch().count();
		printf("Hello world from Mbed CE! - %llu \n", now);
		rtos::ThisThread::sleep_for(1s);
	}
}
