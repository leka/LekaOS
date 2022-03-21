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

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

namespace animations {

	auto thread		 = rtos::Thread {};
	auto event_flags = CoreEventFlags {};

}	// namespace animations

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

}	// namespace leds

auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	ledkit.init();

	hello.start();

	while (true) {
		ledkit.start(&LedKit::animation::sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::afraid_blue);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::afraid_red);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::afraid_red_blue);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::amazed);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::angry);
		rtos::ThisThread::sleep_for(20s);

		ledkit.start(&LedKit::animation::angry_short);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::bubbles);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::disgusted);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::fire);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::fly);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::happy);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::rainbow);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sad);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sad_cry);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sick);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::singing);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sleeping);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sneeze);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::spin_blink);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::sprinkles);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::underwater);
		rtos::ThisThread::sleep_for(20s);

		ledkit.start(&LedKit::animation::wake_up);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::wink);
		rtos::ThisThread::sleep_for(10s);

		ledkit.start(&LedKit::animation::yawn);
		rtos::ThisThread::sleep_for(10s);
	}
}
