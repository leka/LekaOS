// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "AfraidBlue.h"
#include "AfraidRed.h"
#include "AfraidRedBlue.h"
#include "Amazed.h"
#include "Angry.h"
#include "AngryShort.h"
#include "Bubbles.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "Disgusted.h"
#include "Fly.h"
#include "Happy.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LoadingGreen.h"
#include "LoadingOrange.h"
#include "LoadingRed.h"
#include "LoadingYellow.h"
#include "LogKit.h"
#include "Sad.h"
#include "SadCry.h"
#include "Sick.h"
#include "Singing.h"
#include "Sleeping.h"
#include "Sneeze.h"
#include "Underwater.h"
#include "Waiting.h"
#include "WakeUp.h"
#include "Wink.h"
#include "Yawn.h"

using namespace leka;
using namespace std::chrono;

auto corespi_belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};

auto animation_thread	   = rtos::Thread {};
auto animation_event_queue = events::EventQueue {};

auto ledkit = LedKit {animation_thread, animation_event_queue, ears, belt};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		ledkit.start(&LedKit::animation::sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::rainbow);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
