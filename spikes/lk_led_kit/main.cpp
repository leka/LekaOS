// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "Waiting.h"

using namespace leka;
using namespace std::chrono;

auto corespi_belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};

auto animation_thread	   = rtos::Thread {};
auto animation_event_queue = events::EventQueue {};

auto ledkit = LedKit {animation_thread, animation_event_queue, ears, belt};
animation::Waiting animation_waiting {ears, belt};

auto colors_available = std::to_array({
	RGB::pure_green,
	RGB::pure_red,
	RGB::pure_blue,
	RGB::yellow,
	RGB::cyan,
	RGB::magenta,
});

HelloWorld hello;

RGB sick1		   = RGB {30, 255, 0};
RGB sick2		   = RGB {94, 104, 1};
RGB singing_green  = RGB {29, 168, 0};
RGB singing_red	   = RGB {255, 65, 61};
RGB singing_yellow = RGB {255, 217, 61};
RGB singing_blue   = RGB {21, 125, 230};

RGB red_angry = RGB {255, 98, 98};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	auto start	  = rtos::Kernel::Clock::now();
	uint8_t index = 0;

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;

		ledkit.start(animation_waiting);
		rtos::ThisThread::sleep_for(40s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
