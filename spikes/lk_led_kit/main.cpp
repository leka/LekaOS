// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "AfraidBlue.h"
#include "AfraidRed.h"
#include "AfraidRedBlue.h"
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
#include "Singing.h"
#include "Sleeping.h"
#include "WakeUp.h"

using namespace leka;
using namespace std::chrono;

auto corespi_belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};

auto animation_thread	   = rtos::Thread {};
auto animation_event_queue = events::EventQueue {};

auto ledkit = LedKit {animation_thread, animation_event_queue, ears, belt};

led::animation::AfraidBlue animation_afraid_blue(ears, belt);
led::animation::AfraidRed animation_afraid_red(ears, belt);
led::animation::AfraidRedBlue animation_afraid_red_blue(ears, belt);
led::animation::Angry animation_angry(ears, belt);
led::animation::AngryShort animation_angry_short(ears, belt);
led::animation::Bubbles animation_bubbles(ears, belt);
led::animation::Disgusted animation_disgusted(ears, belt);
led::animation::Fly animation_fly {ears, belt};
led::animation::Happy animation_happy(ears, belt);
led::animation::LoadingGreen animation_loading_green(ears, belt);
led::animation::LoadingOrange animation_loading_orange(ears, belt);
led::animation::LoadingRed animation_loading_red(ears, belt);
led::animation::LoadingYellow animation_loading_yellow(ears, belt);
led::animation::Sad animation_sad(ears, belt);
led::animation::SadCry animation_sad_cry(ears, belt);
led::animation::Singing animation_singing(ears, belt);
led::animation::WakeUp animation_wake_up(ears, belt);
led::animation::Sleeping animation_sleeping(ears, belt);

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

		ledkit.start(animation_happy);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_afraid_red);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_sad_cry);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_afraid_blue);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_angry_short);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_angry);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_sad);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_loading_green);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_loading_yellow);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_loading_orange);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_loading_red);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_afraid_red_blue);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(animation_wake_up);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
