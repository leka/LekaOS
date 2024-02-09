// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "CoreQDAC.h"
#include "CoreTouchSensor.h"
#include "DigitalOut.h"
#include "HelloWorld.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono_literals;

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

namespace touch {

inline auto corei2c			  = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
inline auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
inline auto io_expander		  = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

namespace detect_pin {

	inline auto ear_left		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
	inline auto ear_right		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
	inline auto belt_left_front	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
	inline auto belt_left_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
	inline auto belt_right_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
	inline auto belt_right_front = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};

}	// namespace detect_pin

namespace power_mode_pin {

	inline auto ear_left		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
	inline auto ear_right		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
	inline auto belt_left_front	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
	inline auto belt_left_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
	inline auto belt_right_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
	inline auto belt_right_front = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};

}	// namespace power_mode_pin

namespace dac {

	inline auto left  = CoreQDACMCP4728 {corei2c, 0xC2};
	inline auto right = CoreQDACMCP4728 {corei2c, 0xC0};

	namespace channel {

		inline auto ear_left		 = mcp4728::channel::C;
		inline auto ear_right		 = mcp4728::channel::C;
		inline auto belt_left_back	 = mcp4728::channel::A;
		inline auto belt_left_front	 = mcp4728::channel::B;
		inline auto belt_right_back	 = mcp4728::channel::B;
		inline auto belt_right_front = mcp4728::channel::A;

	}	// namespace channel
}	// namespace dac

namespace sensor {

	inline auto ear_left =
		CoreTouchSensor {detect_pin::ear_left, power_mode_pin::ear_left, dac::left, dac::channel::ear_left};
	inline auto ear_right =
		CoreTouchSensor {detect_pin::ear_right, power_mode_pin::ear_right, dac::right, dac::channel::ear_right};
	inline auto belt_left_back = CoreTouchSensor {detect_pin::belt_left_back, power_mode_pin::belt_left_back, dac::left,
												  dac::channel::belt_left_back};
	inline auto belt_left_front	 = CoreTouchSensor {detect_pin::belt_left_front, power_mode_pin::belt_left_front,
													dac::left, dac::channel::belt_left_front};
	inline auto belt_right_back	 = CoreTouchSensor {detect_pin::belt_right_back, power_mode_pin::belt_right_back,
													dac::right, dac::channel::belt_right_back};
	inline auto belt_right_front = CoreTouchSensor {detect_pin::belt_right_front, power_mode_pin::belt_right_front,
													dac::right, dac::channel::belt_right_front};
}	// namespace sensor

}	// namespace touch

auto hello = HelloWorld {};

auto touch_sensor_kit =
	TouchSensorKit {touch::sensor::ear_left,		touch::sensor::ear_right,		touch::sensor::belt_left_back,
					touch::sensor::belt_left_front, touch::sensor::belt_right_back, touch::sensor::belt_right_front};

void printSensorTouched(Position position)
{
	switch (position) {
		case Position::ear_left:
			log_info("Ear left is touched");
			break;
		case Position::ear_right:
			log_info("Ear right is touched");
			break;
		case Position::belt_left_front:
			log_info("Belt left front is touched");
			break;
		case Position::belt_left_back:
			log_info("Belt left back is touched");
			break;
		case Position::belt_right_front:
			log_info("Belt right front is touched");
			break;
		case Position::belt_right_back:
			log_info("Belt right back is touched");
			break;
		default:
			break;
	}
}

void printSensorReleased(Position position)
{
	switch (position) {
		case Position::ear_left:
			log_info("Ear left is released");
			break;
		case Position::ear_right:
			log_info("Ear right is released");
			break;
		case Position::belt_left_front:
			log_info("Belt left front is released");
			break;
		case Position::belt_left_back:
			log_info("Belt left back is released");
			break;
		case Position::belt_right_front:
			log_info("Belt right front is released");
			break;
		case Position::belt_right_back:
			log_info("Belt right back is released");
			break;
		default:
			break;
	}
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	HelloWorld hello {};
	hello.start();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	touch_sensor_kit.registerOnSensorTouched(printSensorTouched);
	touch_sensor_kit.registerOnSensorReleased(printSensorReleased);

	touch_sensor_kit.initialize();
	touch_sensor_kit.setRefreshDelay(20ms);

	log_info("Enable touch for 10s");
	touch_sensor_kit.enable();
	rtos::ThisThread::sleep_for(10s);

	log_info("Disable touch for 10s");
	touch_sensor_kit.disable();
	rtos::ThisThread::sleep_for(10s);

	log_info("Enable Touch");
	touch_sensor_kit.enable();
	{
		log_info("Still alive");
		rtos::ThisThread::sleep_for(5s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
