// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LedKitAnimations.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

namespace {

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

	}	// namespace internal

	namespace animations {

		auto event_loop = EventLoopKit {};

	}	// namespace animations

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::event_loop, leds::ears, leds::belt};

}	// namespace

auto main() -> int
{
	logger::init();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	ledkit.init();

	hello.start();

	while (true) {
		log_info("animation::ble_connection");
		ledkit.start(&led::animation::ble_connection);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sleeping");
		ledkit.start(&led::animation::sleeping);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::afraid_blue");
		ledkit.start(&led::animation::afraid_blue);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::afraid_red");
		ledkit.start(&led::animation::afraid_red);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::afraid_red_blue");
		ledkit.start(&led::animation::afraid_red_blue);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::amazed");
		ledkit.start(&led::animation::amazed);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::angry");
		ledkit.start(&led::animation::angry);
		rtos::ThisThread::sleep_for(20s);

		log_info("animation::angry_short");
		ledkit.start(&led::animation::angry_short);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::blink_green");
		ledkit.start(&led::animation::blink_green);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::bubbles");
		ledkit.start(&led::animation::bubbles);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::disgusted");
		ledkit.start(&led::animation::disgusted);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::fire");
		ledkit.start(&led::animation::fire);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::fly");
		ledkit.start(&led::animation::fly);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::happy");
		ledkit.start(&led::animation::happy);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::rainbow");
		ledkit.start(&led::animation::rainbow);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sad");
		ledkit.start(&led::animation::sad);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sad_cry");
		ledkit.start(&led::animation::sad_cry);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sick");
		ledkit.start(&led::animation::sick);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::singing");
		ledkit.start(&led::animation::singing);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sleeping");
		ledkit.start(&led::animation::sleeping);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sneeze");
		ledkit.start(&led::animation::sneeze);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::spin_blink");
		ledkit.start(&led::animation::spin_blink);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::sprinkles");
		ledkit.start(&led::animation::sprinkles);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::underwater");
		ledkit.start(&led::animation::underwater);
		rtos::ThisThread::sleep_for(20s);

		log_info("animation::wake_up");
		ledkit.start(&led::animation::wake_up);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::wink");
		ledkit.start(&led::animation::wink);
		rtos::ThisThread::sleep_for(2s);

		log_info("animation::yawn");
		ledkit.start(&led::animation::yawn);
		rtos::ThisThread::sleep_for(2s);
	}
}
