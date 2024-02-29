// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CGGraphics.hpp"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreI2C.h"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreLCD.hpp"
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
#include "MotionKit.hpp"
#include "ReinforcerKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

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

namespace sd {

	namespace internal {

		auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

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
	}		// namespace internal

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

namespace display::internal {

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

}	// namespace display::internal

auto videokit	   = VideoKit {display::internal::event_loop, display::internal::corevideo};
auto reinforcerkit = ReinforcerKit {videokit, ledkit, motionkit};

auto hello = HelloWorld {};

}	// namespace

auto main() -> int
{
	logger::init();
	watchdog::start();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	hello.start();

	sd::init();

	rtos::ThisThread::sleep_for(1s);

	ledkit.init();
	videokit.initializeScreen();
	imu::lsm6dsox.init();
	imukit.init();

	rtos::ThisThread::sleep_for(3s);

	{
		reinforcerkit.play(ReinforcerKit::Reinforcer::SpinBlink);
		rtos::ThisThread::sleep_for(1s);
	}

	reinforcerkit.stop();

	display::internal::corelcd.enableDeepSleep();
	motor::left.enableDeepSleep();
	motor::right.enableDeepSleep();

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
