// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "Leds.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

namespace TouchLed {
struct Ear {
	Position position;
	uint8_t index;
};

auto ears = std::array<Ear, 2> {Ear {Position::ear_left, 1}, Ear {Position::ear_right, 0}};

struct Belt {
	Position position;
	uint8_t start_index;
	uint8_t end_index;
};

auto belt = std::array<Belt, 4> {Belt {Position::belt_left_back, 16, 18}, Belt {Position::belt_left_front, 10, 13},
								 Belt {Position::belt_right_back, 6, 9}, Belt {Position::belt_right_front, 1, 3}};

void setColorEar(Ear ear, RGB color)
{
	leds::ears.setColorAtIndex(ear.index, color);
	leds::ears.show();
}

void setColorBelt(Belt belt, RGB color)
{
	leds::belt.setColorRange(belt.start_index, belt.end_index, color);
	leds::belt.show();
}
}	// namespace TouchLed

auto touch_sensor_kit = TouchSensorKit();

void printState()
{
	log_info("Ear left touched: %s", touch_sensor_kit.isTouched(Position::ear_left) ? "true" : "false");
	log_info("Ear right touched: %s", touch_sensor_kit.isTouched(Position::ear_right) ? "true" : "false");
	log_info("Belt left front touched: %s", touch_sensor_kit.isTouched(Position::belt_left_front) ? "true" : "false");
	log_info("Belt left back touched: %s", touch_sensor_kit.isTouched(Position::belt_left_back) ? "true" : "false");
	log_info("Belt right front touched: %s", touch_sensor_kit.isTouched(Position::belt_right_front) ? "true" : "false");
	log_info("Belt right back touched: %s", touch_sensor_kit.isTouched(Position::belt_left_back) ? "true" : "false");
	log_info("\n\n");
}

void updateLeds()
{
	for (TouchLed::Ear touchled: TouchLed::ears) {
		touch_sensor_kit.isTouched(touchled.position) ? TouchLed::setColorEar(touchled, RGB::pure_blue)
													  : TouchLed::setColorEar(touchled, RGB::black);
	}
	for (TouchLed::Belt touchled: TouchLed::belt) {
		touch_sensor_kit.isTouched(touchled.position) ? TouchLed::setColorBelt(touchled, RGB::pure_blue)
													  : TouchLed::setColorBelt(touchled, RGB::black);
	}
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	touch_sensor_kit.init();

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));
	for (Position position: positions) {
		touch_sensor_kit.setSensitivity(position, 0x000F, true);
	}
	rtos::ThisThread::sleep_for(2s);

	auto updateLeds_func = [&]() { updateLeds(); };
	auto printState_func = [&]() { printState(); };

	touch_sensor_kit.resetByPowerMode();
	// touch_sensor_kit.registerOnSensorTouched(printState_func);

	rtos::ThisThread::sleep_for(500ms);

	while (true) {
		updateLeds();
		touch_sensor_kit.resetByPowerMode();
		rtos::ThisThread::sleep_for(1s);
	}
}
