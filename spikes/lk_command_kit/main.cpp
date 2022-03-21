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
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

namespace animations {

	auto thread = rtos::Thread {};
	auto eq		= events::EventQueue {};

}	// namespace animations

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

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

auto hal	  = CoreSTM32Hal {};
auto videokit = VideoKit {hal};

VideoKit_DeclareIRQHandlers(videokit);

auto behaviorkit = BehaviorKit {videokit, ledkit, motor::left, motor::right};

namespace command {

namespace internal {

	auto test		= TestCommand {};
	auto led		= LedSingleCommand {leds::ears, leds::belt};
	auto led_full	= LedFullCommand {leds::ears, leds::belt};
	auto led_range	= LedRangeCommand {leds::ears, leds::belt};
	auto motors		= MotorsCommand {motor::left, motor::right};
	auto reinforcer = ReinforcerCommand {behaviorkit};

}	// namespace internal

namespace data {

	// clang-format off

	auto test_cmd_0 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x8F});
	auto test_cmd_1 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});

	auto led_single_ears_0 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x8F});
	auto led_single_belt_3 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x10, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});

	auto led_full_ears = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x13, 0x14, 0x00, 0x7F, 0xFF, 0x92});
	auto led_full_belt = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x13, 0x15, 0x00, 0x7F, 0xFF, 0x93});

	auto led_range_ears = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x16, 0x17, 0x00, 0x01, 0x00, 0x7F, 0xFF, 0x96});
	auto led_range_belt = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x16, 0x18, 0x04, 0x0A, 0x00, 0x7F, 0xFF, 0xA4});

	auto motors_forward  = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x02, 0x20, 0x21, 0x01, 0xFF, 0x21, 0x20, 0x22, 0x01, 0xFF, 0x22});
	auto motors_backward = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x02, 0x20, 0x21, 0x00, 0xFF, 0x20, 0x20, 0x22, 0x00, 0xFF, 0x21});

	auto reinforcer_rainbow   = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x50, 0x51, 0x51});
	auto reinforcer_sprinkles = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x50, 0x53, 0x53});

	// clang-format on

	auto list = std::to_array<std::span<uint8_t>>({
		test_cmd_0,
		test_cmd_1,
		led_single_ears_0,
		led_single_belt_3,
		led_full_ears,
		led_full_belt,
		led_range_ears,
		led_range_belt,
		motors_forward,
		motors_backward,
		reinforcer_rainbow,
		reinforcer_sprinkles,
	});

}	// namespace data

auto list = std::to_array<interface::Command *>({
	&internal::test,
	&internal::led,
	&internal::led_full,
	&internal::led_range,
	&internal::reinforcer,
});

}	// namespace command

void turnOff()
{
	log_debug("turn off start");
	leds::ears.setColor(RGB::black);
	leds::ears.show();
	leds::belt.setColor(RGB::black);
	leds::belt.show();
	behaviorkit.stop();
	log_debug("turn off end");
};

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto cmdkit = CommandKit {};

HelloWorld hello;

auto main() -> int
{
	logger::init();

	initializeSD();
	videokit.initializeScreen();
	cmdkit.registerCommand(command::list);

	turnOff();

	log_info();
	log_info();
	log_info("Hello, spike CommandKit!");
	log_info();
	log_info();

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));

		for (auto i = 0; const auto &data: command::data::list) {
			log_debug("index: %i", i++);
			cmdkit.push(data);
			rtos::ThisThread::sleep_for(1500ms);
			turnOff();
		}
	}
}
