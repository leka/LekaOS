// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <random>

#include "rtos/ThisThread.h"

#include "BehaviorKit.h"
#include "CommandKit.h"
#include "CoreLED.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

namespace animations {

	auto thread = rtos::Thread {};
	auto eq		= events::EventQueue {};

}	// namespace animations

}	// namespace leds

auto ledkit = LedKit {leds::animations::thread, leds::animations::eq, leds::ears, leds::belt};

namespace motor {

namespace internal {

	namespace left {

		auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_LEFT_PWM};

	}	// namespace left
	namespace right {

		auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_RIGHT_PWM};

	}	// namespace right
}	// namespace internal

auto left  = CoreMotor {internal::left::dir_1, internal::left::dir_2, internal::left::speed};
auto right = CoreMotor {internal::right::dir_1, internal::right::dir_2, internal::right::speed};

}	// namespace motor

// auto videokit	 = VideoKit {};
// auto behaviorkit = BehaviorKit {videokit, ledkit, motor::left, motor::right};

namespace command {

namespace internal {

	auto led = LedCommand {leds::ears, leds::belt};

}

namespace data {

	// clang-format off

	auto led_ears_0         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x8F});
	auto led_belt_3         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});

	auto bad_chcksm         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x00});
	auto start_frame_only   = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A,                                               });

	auto bad_missing_data_1 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00, 0x7F, 0xFF,     });
	auto bad_missing_data_2 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00, 0x7F,           });
	auto bad_missing_data_3 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00,                 });
	auto bad_missing_data_4 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00,                       });

	auto random_data        = std::to_array<uint8_t>({0x43, 0x2A, 0xB4, 0xCC, 0x54, 0x10, 0x11, 0x22, 0x9F, 0x00, 0xFF, 0x8F});

	// clang-format on

	auto all = std::to_array<std::span<uint8_t>>({
		led_ears_0,
		random_data,
		led_belt_3,
		bad_chcksm,
		start_frame_only,
		random_data,
		bad_missing_data_1,
		random_data,
		bad_missing_data_2,
		led_belt_3,
		bad_missing_data_3,
		random_data,
		bad_missing_data_4,
		random_data,
		led_ears_0,
	});

	auto working = std::to_array<std::span<uint8_t>>({led_ears_0, random_data, led_belt_3, bad_chcksm, random_data,
													  led_belt_3, random_data, random_data, led_ears_0, random_data});

	auto random = std::to_array<std::span<uint8_t>>({random_data, random_data, random_data, random_data, random_data,
													 random_data, random_data, random_data, random_data, random_data});

}	// namespace data

auto list = std::to_array<interface::Command *>({&internal::led});

}	// namespace command

auto cmdkit = CommandKit {};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	cmdkit.registerCommand(command::list);

	log_info("Hello, spike CommandKit!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));

		std::random_device rd;
		std::minstd_rand g(rd());

		// std::shuffle(command::data::all.begin(), command::data::all.end(), g);

		for (auto i = 0; const auto &data: command::data::all) {
			log_debug("index: %i", i++);
			cmdkit.push(data);
		}

		rtos::ThisThread::sleep_for(1s);
	}
}
