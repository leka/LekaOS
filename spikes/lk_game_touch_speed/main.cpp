// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <utility>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LedManager.h"
#include "Level.h"
#include "LogKit.h"
#include "Round.h"
#include "TouchSensorKit.h"
using namespace leka;
using namespace std::chrono;

namespace leds {

auto touch_sensor_kit = TouchSensorKit();

}	// namespace leds
auto main() -> int
{
	CoreSPI spiEars {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
	CoreSPI spiBelt {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	CoreLED<2> ears(spiEars);
	CoreLED<20> belt(spiBelt);
	EventLoopKit loopKit {};
	TouchSensorKit touchSensorKit {};
	LedKit ledKit(loopKit, ears, belt);
	leds::LedManager ledManager(ears, belt, ledKit);
	leds::Round round(ledManager, touchSensorKit);

	logger::init();
	ledManager.turnOff();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	leds::touch_sensor_kit.setup();

	rtos::ThisThread::sleep_for(2s);

	leds::touch_sensor_kit.adjust_sensivity(0x00);

	uint8_t level = 0;
	while (true) {
		leds::Level startLevel(ledManager, level, round, touchSensorKit);

		startLevel.playAllRounds();
		++level;
		if (level == 4) {
			level = 0;
		}
	}
}
