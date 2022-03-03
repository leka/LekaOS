// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "Bubbles.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "Fly.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "Singing.h"

using namespace leka;
using namespace std::chrono;

auto corespi_belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};

auto animation_thread	   = rtos::Thread {};
auto animation_event_queue = events::EventQueue {};

auto ledkit = LedKit {animation_thread, animation_event_queue, ears, belt};

led::animation::Bubbles animation_bubbles(ears, belt);
led::animation::Singing animation_singing(ears, belt);
led::animation::Fly animation_fly {ears, belt};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		ledkit.start(animation_singing);
		rtos::ThisThread::sleep_for(10s);
		ledkit.start(animation_fly);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
