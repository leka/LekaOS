// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <random>

#include "rtos/ThisThread.h"

#include "CommandKit.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreI2C.h"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLED.h"
#include "CoreLL.h"
#include "CoreLSM6DSOX.hpp"
#include "CoreLTDC.hpp"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSDRAM.hpp"
#include "CoreSPI.h"
#include "CoreSTM32Hal.h"
#include "CoreTimeout.h"
#include "CoreVideo.hpp"
#include "EventLoopKit.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LedKit.h"
#include "LogKit.h"
#include "ReinforcerKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"
#include "commands/LedFullCommand.h"
#include "commands/LedRangeCommand.h"
#include "commands/LedSingleCommand.h"
#include "commands/MotorsCommand.h"
#include "commands/ReinforcerCommand.h"
#include "commands/TestCommand.h"

using namespace leka;
using namespace std::chrono;

#include "mbed_stats.h"

#include "drivers/Watchdog.h"
namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

		namespace stats {

			auto cpu   = mbed_stats_cpu_t {};
			auto stack = mbed_stats_stack_t {};
			auto heap  = mbed_stats_heap_t {};

		}	// namespace stats

		__attribute__((noreturn)) void watchdog_kick()
		{
			static auto kick_count = uint32_t {0};

			static auto start = rtos::Kernel::Clock::now();
			static auto stop  = rtos::Kernel::Clock::now();
			static auto delta = static_cast<int>((stop - start).count());

			static auto idle_ratio		 = uint8_t {};
			static auto sleep_ratio		 = uint8_t {};
			static auto deep_sleep_ratio = uint8_t {};

			static auto stack_used_delta	= int32_t {};
			static auto stack_used_size		= uint32_t {};
			static auto stack_reserved_size = uint32_t {};
			static auto stack_used_ratio	= uint8_t {};

			static auto heap_used_delta	   = int32_t {};
			static auto heap_used_size	   = uint32_t {};
			static auto heap_reserved_size = uint32_t {};
			static auto heap_used_ratio	   = uint8_t {};

			while (true) {
				internal::instance.kick();
				++kick_count;

				stop  = rtos::Kernel::Clock::now();
				delta = static_cast<int>((stop - start).count());

				mbed_stats_cpu_get(&stats::cpu);

				idle_ratio	= static_cast<uint8_t>(((stats::cpu.idle_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				deep_sleep_ratio =
					static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

				mbed_stats_stack_get(&stats::stack);

				stack_used_delta	= static_cast<int32_t>(stats::stack.max_size - stack_used_size);
				stack_used_size		= stats::stack.max_size;
				stack_reserved_size = stats::stack.reserved_size;
				stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

				mbed_stats_heap_get(&stats::heap);

				heap_used_delta	   = static_cast<int32_t>(stats::heap.current_size - heap_used_size);
				heap_used_size	   = stats::heap.current_size;
				heap_reserved_size = stats::heap.reserved_size;
				heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

				log_info(
					"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
					stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size,
					heap_reserved_size);

				start = rtos::Kernel::Clock::now();
				rtos::ThisThread::sleep_for(5s);
			}
		}

	}	// namespace internal

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::internal::watchdog_kick);
	}

}	// namespace watchdog

}	// namespace

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

namespace animations {

	auto event_loop = EventLoopKit {};

}	// namespace animations

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

}	// namespace leds

auto ledkit = LedKit {leds::animations::event_loop, leds::ears, leds::belt};

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

namespace imu {
namespace internal {

	auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
	auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

}	// namespace internal

CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

auto imukit = IMUKit {imu::lsm6dsox};

namespace motion::internal {

CoreTimeout timeout {};

}	// namespace motion::internal

auto motionkit = MotionKit {motor::left, motor::right, imukit, motion::internal::timeout};

namespace display {

namespace internal {

	auto event_loop = EventLoopKit {};
	auto backlight	= CorePwm {SCREEN_BACKLIGHT_PWM};

	auto corell		   = CoreLL {};
	auto pixel		   = CGPixel {corell};
	auto hal		   = CoreSTM32Hal {};
	auto coresdram	   = CoreSDRAM {hal};
	auto coredma2d	   = CoreDMA2D {hal};
	auto coredsi	   = CoreDSI {hal};
	auto coreltdc	   = CoreLTDC {hal};
	auto coregraphics  = CoreGraphics {coredma2d};
	auto corefont	   = CoreFont {pixel};
	auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, backlight};
	auto corelcd	   = CoreLCD {coreotm};
	auto _corejpegmode = CoreJPEGModeDMA {hal};
	auto corejpeg	   = CoreJPEG {hal, _corejpegmode};

	extern "C" auto corevideo =
		CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

}	// namespace internal

auto videokit = VideoKit {internal::event_loop, internal::corevideo};

}	// namespace display

auto reinforcerkit = ReinforcerKit {display::videokit, ledkit, motionkit};

namespace command {

namespace internal {

	auto event_loop = EventLoopKit {};

	auto test		= TestCommand {};
	auto led		= LedSingleCommand {leds::ears, leds::belt};
	auto led_full	= LedFullCommand {leds::ears, leds::belt};
	auto led_range	= LedRangeCommand {leds::ears, leds::belt};
	auto motors		= MotorsCommand {motor::left, motor::right};
	auto reinforcer = ReinforcerCommand {reinforcerkit};

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
	reinforcerkit.stop();
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

auto cmdkit = CommandKit {command::internal::event_loop};

HelloWorld hello;

auto main() -> int
{
	logger::init();
	watchdog::start();

	initializeSD();
	display::videokit.initializeScreen();
	display::internal::corelcd.turnOn();
	cmdkit.registerCommand(command::list);

	imu::lsm6dsox.init();
	imukit.init();

	turnOff();

	log_info();
	log_info();
	log_info("Hello, spike CommandKit!");
	log_info();
	log_info();

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	{
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));

		for (auto i = 0; const auto &data: command::data::list) {
			log_debug("index: %i", i++);
			cmdkit.push(data);
			rtos::ThisThread::sleep_for(1500ms);
			turnOff();
		}
	}

	display::internal::corelcd.enableDeepSleep();
	motor::left.enableDeepSleep();
	motor::right.enableDeepSleep();

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
