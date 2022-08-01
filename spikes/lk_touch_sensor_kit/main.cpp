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
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

namespace leka::touch {

inline auto corei2c			  = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
inline auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
inline auto io_expander		  = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

namespace detect_pin {
	inline auto ear_left		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
	inline auto ear_right		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
	inline auto belt_left_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
	inline auto belt_left_front	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
	inline auto belt_right_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
	inline auto belt_right_front = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};
}	// namespace detect_pin

namespace power_mode_pin {
	inline auto ear_left		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
	inline auto ear_right		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
	inline auto belt_left_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
	inline auto belt_left_front	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
	inline auto belt_right_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
	inline auto belt_right_front = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};
}	// namespace power_mode_pin

namespace dac {
	inline auto left  = CoreQDACMCP4728 {corei2c, 0xC0};
	inline auto right = CoreQDACMCP4728 {corei2c, 0xC2};
	namespace channel {
		inline auto ear_left		 = Channel::C;
		inline auto ear_right		 = Channel::C;
		inline auto belt_left_back	 = Channel::B;
		inline auto belt_left_front	 = Channel::A;
		inline auto belt_right_back	 = Channel::B;
		inline auto belt_right_front = Channel::A;
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

namespace position {
	inline constexpr auto ear_left		   = uint8_t {0};
	inline constexpr auto ear_right		   = uint8_t {1};
	inline constexpr auto belt_left_back   = uint8_t {2};
	inline constexpr auto belt_left_front  = uint8_t {3};
	inline constexpr auto belt_right_back  = uint8_t {4};
	inline constexpr auto belt_right_front = uint8_t {5};
}	// namespace position

}	// namespace leka::touch

auto event_loop		  = EventLoopKit {};
auto touch_sensor_kit = TouchSensorKit {event_loop,
										touch::sensor::ear_left,
										touch::sensor::ear_right,
										touch::sensor::belt_left_front,
										touch::sensor::belt_left_back,
										touch::sensor::belt_right_front,
										touch::sensor::belt_left_back};

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
	log_info("\n\n");
	rtos::ThisThread::sleep_for(1s);
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
	}
	log_info("\n\n");
	rtos::ThisThread::sleep_for(1s);
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));

	rtos::ThisThread::sleep_for(2s);

	touch_sensor_kit.init();

	auto printSensorTouched_func  = [&](const Position position) { printSensorTouched(position); };
	auto printSensorReleased_func = [&](const Position position) { printSensorReleased(position); };

	touch_sensor_kit.registerOnSensorTouched(printSensorTouched_func);
	touch_sensor_kit.registerOnSensorReleased(printSensorReleased_func);

	touch_sensor_kit.start();

	while (true) {
		rtos::ThisThread::sleep_for(100ms);
	}
}
