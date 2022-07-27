// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreTouchSensor.h"
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

auto ears = std::array<Ear, 2> {Ear {Position::ear_right, 0}, Ear {Position::ear_left, 1}};

struct Belt {
	Position position;
	uint8_t start_index;
	uint8_t end_index;
};

auto belt = std::array<Belt, 4> {Belt {Position::belt_left_front, 10, 13}, Belt {Position::belt_left_back, 16, 18},
								 Belt {Position::belt_right_front, 1, 3}, Belt {Position::belt_right_back, 6, 9}};

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

void turnOnLeds(Position position)
{
	for (Ear sensor: ears) {
		if (sensor.position == position) {
			TouchLed::setColorEar(sensor, RGB::pure_blue);
		}
	}
	for (Belt sensor: belt) {
		if (sensor.position == position) {
			TouchLed::setColorBelt(sensor, RGB::pure_blue);
		}
	}
}

void turnOffLeds(Position position)
{
	for (Ear sensor: ears) {
		if (sensor.position == position) {
			TouchLed::setColorEar(sensor, RGB::black);
		}
	}
	for (Belt sensor: belt) {
		if (sensor.position == position) {
			TouchLed::setColorBelt(sensor, RGB::black);
		}
	}
}

void turnOnAllLeds()
{
	for (Ear sensor: ears) {
		TouchLed::setColorEar(sensor, RGB::pure_blue);
	}
	for (Belt sensor: belt) {
		TouchLed::setColorBelt(sensor, RGB::pure_blue);
	}
}
void turnOffAllLeds()
{
	for (Ear sensor: ears) {
		TouchLed::setColorEar(sensor, RGB::black);
	}
	for (Belt sensor: belt) {
		TouchLed::setColorBelt(sensor, RGB::black);
	}
}

}	// namespace TouchLed

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

void calibration()
{
	TouchLed::turnOffAllLeds();

	log_info("Put your hand on ear left sensor");
	touch_sensor_kit.calibrate(Position::ear_left);
	setColorEar(TouchLed::ears.at(1), RGB::pure_red);

	log_info("Put your hand on ear right sensor");
	touch_sensor_kit.calibrate(Position::ear_right);
	setColorEar(TouchLed::ears.at(0), RGB::pure_red);

	log_info("Put your hand on belt left front sensor");
	touch_sensor_kit.calibrate(Position::belt_left_front);
	setColorBelt(TouchLed::belt.at(0), RGB::pure_red);

	log_info("Put your hand on belt left back sensor");
	touch_sensor_kit.calibrate(Position::belt_left_back);
	setColorBelt(TouchLed::belt.at(1), RGB::pure_red);

	log_info("Put your hand on belt right front sensor");
	touch_sensor_kit.calibrate(Position::belt_right_front);
	setColorBelt(TouchLed::belt.at(2), RGB::pure_red);

	log_info("Put your hand on belt right back sensor");
	touch_sensor_kit.calibrate(Position::belt_right_back);
	setColorBelt(TouchLed::belt.at(3), RGB::pure_red);

	TouchLed::turnOffAllLeds();
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

	TouchLed::turnOffAllLeds();

	touch_sensor_kit.init();
	calibration();

	auto turnOnLeds_func  = [&](Position position) { TouchLed::turnOnLeds(position); };
	auto turnOffLeds_func = [&](Position position) { TouchLed::turnOffLeds(position); };

	touch_sensor_kit.registerOnSensorTouched(turnOnLeds_func);
	touch_sensor_kit.registerOnSensorReleased(turnOffLeds_func);

	touch_sensor_kit.start();

	while (true) {
		rtos::ThisThread::sleep_for(100ms);
	}
}
