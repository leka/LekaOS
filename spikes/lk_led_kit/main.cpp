// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto corespi_belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};

auto animation_thread = rtos::Thread {};
auto event_flags	  = CoreEventFlags {};

auto ledkit = LedKit {animation_thread, event_flags, ears, belt};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	ledkit.init();

	hello.start();

	while (true) {
		ledkit.start(&LedKit::animation::sleeping);

		rtos::ThisThread::sleep_for(3s);
		ledkit.start(&LedKit::animation::blink_green);

		rtos::ThisThread::sleep_for(10s);
		ledkit.start(&LedKit::animation::spin_blink);

		rtos::ThisThread::sleep_for(10s);
		ledkit.start(&LedKit::animation::fire);

		rtos::ThisThread::sleep_for(10s);
		ledkit.start(&LedKit::animation::sprinkles);

		rtos::ThisThread::sleep_for(10s);
		ledkit.start(&LedKit::animation::rainbow);

		rtos::ThisThread::sleep_for(10s);
	}
}
