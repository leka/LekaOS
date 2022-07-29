// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "GameKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "games/GameSpeed.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	CoreSPI spiEars {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
	CoreSPI spiBelt {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	CoreLED<2> ears(spiEars);
	CoreLED<20> belt(spiBelt);
	auto event_loop_ledkit	= EventLoopKit {};
	auto event_loop_gamekit = EventLoopKit {};

	auto ledkit	 = LedKit {event_loop_ledkit, ears, belt};
	auto gamekit = GameKit {event_loop_gamekit};
	leds::LedManager ledmanager(ears, belt, ledkit);

	auto touch_sensor_kit = TouchSensorKit {};
	GameSpeed gamespeed(ledmanager, 0, touch_sensor_kit);

	logger::init();

	log_info("Hello, World!\n\n");

	gamekit.init();

	while (true) {
		gamekit.start(&gamespeed);
		rtos::ThisThread::sleep_for(10s);
	}
}
