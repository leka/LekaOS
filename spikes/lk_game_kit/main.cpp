// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "CoreLED.h"
#include "CoreQDAC.h"
#include "CoreSPI.h"
#include "CoreTouchSensor.h"
#include "EventLoopKit.h"
#include "GameKit.h"
#include "HelloWorld.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LedKit.h"
#include "LogKit.h"
#include "TouchSensorKit.h"
#include "games/GameSpeed.h"

using namespace leka;
using namespace std::chrono;

inline auto corei2c			  = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
inline auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
inline auto io_expander		  = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};
namespace detect_pin {
inline auto ear_left		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
inline auto ear_right		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
inline auto belt_left_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
inline auto belt_left_front	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
inline auto belt_right_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
inline auto belt_right_front = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};
}	// namespace detect_pin

namespace power_mode_pin {
inline auto ear_left		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
inline auto ear_right		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
inline auto belt_left_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
inline auto belt_left_front	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
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
inline auto belt_left_back	= CoreTouchSensor {detect_pin::belt_left_back, power_mode_pin::belt_left_back, dac::left,
											   dac::channel::belt_left_back};
inline auto belt_left_front = CoreTouchSensor {detect_pin::belt_left_front, power_mode_pin::belt_left_front, dac::left,
											   dac::channel::belt_left_front};
inline auto belt_right_back = CoreTouchSensor {detect_pin::belt_right_back, power_mode_pin::belt_right_back, dac::right,
											   dac::channel::belt_right_back};
inline auto belt_right_front = CoreTouchSensor {detect_pin::belt_right_front, power_mode_pin::belt_right_front,
												dac::right, dac::channel::belt_right_front};
}	// namespace sensor

auto event_loop		  = EventLoopKit {};
auto touch_sensor_kit = TouchSensorKit {event_loop,
										sensor::ear_left,
										sensor::ear_right,
										sensor::belt_left_back,
										sensor::belt_left_front,
										sensor::belt_right_back,
										sensor::belt_right_front};

auto main() -> int
{
	CoreSPI spiEars {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
	CoreSPI spiBelt {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	CoreLED<2> ears(spiEars);
	CoreLED<20> belt(spiBelt);
	auto event_loop_ledkit	= EventLoopKit {};
	auto event_loop_gamekit = EventLoopKit {};

	auto ledkit	 = LedKit {event_loop_ledkit, ears, belt};
	auto gamekit = GameKit {event_loop_gamekit};
	leds::LedManager ledmanager(ears, belt, ledkit);

	GameSpeed gamespeed(ledmanager, 0, touch_sensor_kit);

	logger::init();

	log_info("Hello, World!\n\n");

	gamekit.init();

	while (true) {
		gamekit.start(&gamespeed);
		rtos::ThisThread::sleep_for(30s);
	}
}
