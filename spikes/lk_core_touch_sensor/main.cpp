// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <cstddef>

#include "PinNames.h"

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

namespace touch {

auto corei2c		   = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
auto io_expander	   = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

namespace detect_pin {

	auto ear_left		  = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
	auto ear_right		  = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
	auto belt_left_front  = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
	auto belt_left_back	  = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
	auto belt_right_back  = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
	auto belt_right_front = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};

}	// namespace detect_pin

namespace power_mode_pin {

	auto ear_left		  = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
	auto ear_right		  = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
	auto belt_left_front  = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
	auto belt_left_back	  = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
	auto belt_right_back  = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
	auto belt_right_front = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};

}	// namespace power_mode_pin

namespace dac {

	auto left  = CoreQDACMCP4728 {corei2c, 0xC2};
	auto right = CoreQDACMCP4728 {corei2c, 0xC0};
	namespace channel {

		auto ear_left		  = mcp4728::channel::C;
		auto ear_right		  = mcp4728::channel::C;
		auto belt_left_back	  = mcp4728::channel::A;
		auto belt_left_front  = mcp4728::channel::B;
		auto belt_right_back  = mcp4728::channel::B;
		auto belt_right_front = mcp4728::channel::A;

	}	// namespace channel

}	// namespace dac

namespace sensor {

	auto ear_left = CoreTouchSensor {detect_pin::ear_left, power_mode_pin::ear_left, dac::left, dac::channel::ear_left};
	auto ear_right =
		CoreTouchSensor {detect_pin::ear_right, power_mode_pin::ear_right, dac::right, dac::channel::ear_right};
	auto belt_left_back	  = CoreTouchSensor {detect_pin::belt_left_back, power_mode_pin::belt_left_back, dac::left,
											 dac::channel::belt_left_back};
	auto belt_left_front  = CoreTouchSensor {detect_pin::belt_left_front, power_mode_pin::belt_left_front, dac::left,
											 dac::channel::belt_left_front};
	auto belt_right_back  = CoreTouchSensor {detect_pin::belt_right_back, power_mode_pin::belt_right_back, dac::right,
											 dac::channel::belt_right_back};
	auto belt_right_front = CoreTouchSensor {detect_pin::belt_right_front, power_mode_pin::belt_right_front, dac::right,
											 dac::channel::belt_right_front};
}	// namespace sensor

}	// namespace touch

namespace sensitivity {

inline constexpr auto low	 = float {0.5F};
inline constexpr auto medium = float {0.7F};
inline constexpr auto high	 = float {1.0F};

}	// namespace sensitivity

auto sensor = CoreTouchSensor {touch::sensor::ear_left};

auto resetByTouchAndRelease()
{
	log_info("Put your hand on the sensor then release !\n\n");

	rtos::ThisThread::sleep_for(1s);

	while (sensor.read()) {
		log_info("Waiting for hand...");
		rtos::ThisThread::sleep_for(100ms);
	}

	log_info("OK !\n\n");
}

void calibration()
{
	log_info("Start calibration in 5s...");
	log_info("Put your hand on the sensor\n\n");

	rtos::ThisThread::sleep_for(5s);

	auto value = float {CoreTouchSensor::default_min_sensitivity_input_value};
	auto step  = float {0.1F};

	constexpr auto accurate_read_count = uint8_t {10};
	auto read_count					   = uint8_t {0};

	sensor.setSensitivity(value);

	while (read_count < accurate_read_count && value <= CoreTouchSensor::default_max_sensitivity_input_value) {
		if (!sensor.read()) {
			read_count = 0;
			value += step;
			sensor.setSensitivity(value);
			log_info(" Value : %d\n\n", value);

		} else {
			read_count++;
		}
		rtos::ThisThread::sleep_for(100ms);
	}
	sensor.setSensitivity(value);
	log_info("Final sensitivity Value : %d\n\n", value);
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	HelloWorld hello;
	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	sensor.init();
	sensor.setSensitivity(sensitivity::medium);

	// ? Current workaround for bug #1112
	// TODO(@leka/dev-embedded) - Try to find solution w/ software or ui/ux
	resetByTouchAndRelease();

	// ? To turn on calibration, uncomment line below
	// calibration();

	log_info("Touch the sensor ! \n\n");

	rtos::ThisThread::sleep_for(1s);

	{
		if (auto is_touched = sensor.read(); is_touched) {
			log_info("Sensor touched");
		} else {
			log_info(".");
		}

		rtos::ThisThread::sleep_for(100ms);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
