// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreTimeout.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LogKit.h"
#include "MotionKit.hpp"
#include "RFIDKit.h"

using namespace std::chrono;
using namespace leka;

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

namespace motors {

	namespace left {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace left

	namespace right {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_RIGHT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace right

}	// namespace motors

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		auto event_queue = CoreEventQueue();

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

auto imukit = IMUKit {imu::lsm6dsox};

namespace motion::internal {

	CoreTimeout timeout {};

}	// namespace motion::internal

auto motionkit = MotionKit {motors::left::motor, motors::right::motor, imukit, motion::internal::timeout};

namespace rfid {

	auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto reader = CoreRFIDReaderCR95HF(serial);

}	// namespace rfid

auto rfidkit = RFIDKit(rfid::reader);

}	// namespace

void onMagicCardAvailable(const MagicCard &card)
{
	switch (card.getId()) {
		case (MagicCard::number_1.getId()):
			motionkit.startYawRotation(90, Rotation::counterClockwise, [] { log_debug("Callback end of rotation"); });
			break;
		case (MagicCard::number_2.getId()):
			motionkit.startYawRotation(180, Rotation::clockwise);
			break;
		case (MagicCard::number_3.getId()):
			motionkit.startYawRotation(360, Rotation::counterClockwise);
			break;
		case (MagicCard::number_4.getId()):
			motionkit.startYawRotation(540, Rotation::clockwise);
			break;
		case (MagicCard::number_5.getId()):
			motionkit.startYawRotation(720, Rotation::counterClockwise);
			break;
		case (MagicCard::number_6.getId()):
			motionkit.startYawRotation(1080, Rotation::clockwise);
			break;
		case (MagicCard::number_7.getId()):
			motionkit.startYawRotation(1080, Rotation::counterClockwise);
			break;
		case (MagicCard::number_10.getId()):
			motionkit.stop();
			break;
		default:
			motionkit.stop();
			log_debug("Stop");
			break;
	}
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();
	imukit.init();
	rfidkit.init();

	rfidkit.onTagActivated(onMagicCardAvailable);

	{
		rtos::ThisThread::sleep_for(100ms);
	}

	rfidkit.enableDeepSleep();
	motors::left::motor.enableDeepSleep();
	motors::right::motor.enableDeepSleep();

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
