// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LekaBluetooth.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto audio_enable = mbed::DigitalOut {SOUND_ENABLE, 1};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	Bluetooth bluetooth;

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	bluetooth.start();

	while (true) {
		rtos::ThisThread::sleep_for(10s);
	}
}
