// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto blekit = BLEKit {};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	blekit.init();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
