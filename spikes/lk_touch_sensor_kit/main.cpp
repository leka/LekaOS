// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

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

	namespace animations {

		auto event_loop = EventLoopKit {};

	}	// namespace animations

}	// namespace internal

auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

void turnOn()
{
	ears.setColor(RGB::pure_blue);
	belt.setColor(RGB::pure_blue);
	ears.show();
	belt.show();
}
void turnOff()
{
	ears.setColor(RGB::black);
	belt.setColor(RGB::black);
	ears.show();
	belt.show();
}
void turnOnEarLeft(RGB color)
{
	ears.setColorAtIndex(1, color);
	ears.show();
}
void turnOffEarLeft()
{
	ears.setColorAtIndex(1, RGB::black);
	ears.show();
}
void turnOnEarRight(RGB color)
{
	ears.setColorAtIndex(0, color);
	ears.show();
}
void turnOffEarRight()
{
	ears.setColorAtIndex(0, RGB::black);
	ears.show();
}
void turnOnBeltRightBack(RGB color)
{
	belt.setColorAtIndex(1, color);
	belt.setColorAtIndex(2, color);
	belt.setColorAtIndex(3, color);
	belt.show();
}
void turnOffBeltRightBack()
{
	belt.setColorAtIndex(1, RGB::black);
	belt.setColorAtIndex(2, RGB::black);
	belt.setColorAtIndex(3, RGB::black);
	belt.show();
}
void turnOnBeltRightFront(RGB color)
{
	belt.setColorAtIndex(6, color);
	belt.setColorAtIndex(7, color);
	belt.setColorAtIndex(8, color);
	belt.setColorAtIndex(9, color);
	belt.show();
}
void turnOffBeltRightFront()
{
	belt.setColorAtIndex(6, RGB::black);
	belt.setColorAtIndex(7, RGB::black);
	belt.setColorAtIndex(8, RGB::black);
	belt.setColorAtIndex(9, RGB::black);
	belt.show();
}
void turnOnBeltLeftBack(RGB color)
{
	belt.setColorAtIndex(10, color);
	belt.setColorAtIndex(11, color);
	belt.setColorAtIndex(12, color);
	belt.setColorAtIndex(13, color);
	belt.show();
}
void turnOffBeltLeftBack()
{
	belt.setColorAtIndex(10, RGB::black);
	belt.setColorAtIndex(11, RGB::black);
	belt.setColorAtIndex(12, RGB::black);
	belt.setColorAtIndex(13, RGB::black);
	belt.show();
}
void turnOnBeltLeftFront(RGB color)
{
	belt.setColorAtIndex(16, color);
	belt.setColorAtIndex(17, color);
	belt.setColorAtIndex(18, color);
	belt.show();
}
void turnOffBeltLeftFront()
{
	belt.setColorAtIndex(16, RGB::black);
	belt.setColorAtIndex(17, RGB::black);
	belt.setColorAtIndex(18, RGB::black);
	belt.show();
}

auto random_numbers = std::vector<uint8_t>();	// tableau qui décrit l'état des 6 pins au cours du temps
auto random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}

void setLedsWithRandomColors()
{
	random_numbers.clear();
	std::vector<RGB> colors;
	for (int i(0); i < 6; i++) {
		auto random_number = random8(0, 1);
		random_numbers.push_back(random_number);
		colors.push_back(random_number == 0 ? RGB::pure_blue : RGB::pure_red);
	}
	turnOnEarLeft(colors[0]);
	turnOnEarRight(colors[1]);
	turnOnBeltRightBack(colors[2]);
	turnOnBeltRightFront(colors[3]);
	turnOnBeltLeftBack(colors[4]);
	turnOnBeltLeftFront(colors[5]);
}

void turnOffAllLeds()
{
	turnOffEarLeft();
	turnOffEarRight();
	turnOffBeltRightBack();
	turnOffBeltRightFront();
	turnOffBeltLeftBack();
	turnOffBeltLeftFront();
}

}	// namespace leds

void launch_sequence(uint8_t seq)
{
	auto digit = uint8_t {0};

	for (uint8_t i(0); i < 8; i++) {
		digit = 1 & (seq >> i);
		leds::ears.setColorAtIndex(digit, RGB::pure_blue);
		leds::ears.show();
		rtos::ThisThread::sleep_for(1s);
		leds::ears.setColorAtIndex(digit, RGB::black);
		leds::ears.show();
		rtos::ThisThread::sleep_for(1s);
	}
}

void start_animation()
{
	for (uint8_t i(0); i < 2; i++) {
		leds::ears.setColorAtIndex(0, RGB::pure_blue);
		leds::ears.setColorAtIndex(1, RGB::pure_blue);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
		leds::ears.setColorAtIndex(0, RGB::black);
		leds::ears.setColorAtIndex(1, RGB::black);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
	}
}

void failed_animation()
{
	for (uint8_t i(0); i < 2; i++) {
		leds::ears.setColorAtIndex(0, RGB::pure_red);
		leds::ears.setColorAtIndex(1, RGB::pure_red);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
		leds::ears.setColorAtIndex(0, RGB::black);
		leds::ears.setColorAtIndex(1, RGB::black);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
	}
}

void win_animation()
{
	for (uint8_t i(0); i < 2; i++) {
		leds::ears.setColorAtIndex(0, RGB::pure_green);
		leds::ears.setColorAtIndex(1, RGB::pure_green);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
		leds::ears.setColorAtIndex(0, RGB::black);
		leds::ears.setColorAtIndex(1, RGB::black);
		leds::ears.show();
		rtos::ThisThread::sleep_for(500ms);
	}
}

auto ledkit = LedKit {leds::internal::animations::event_loop, leds::ears, leds::belt};

auto touch_sensor_kit = TouchSensorKit();

void update_leds()
{
	touch_sensor_kit.updateState();
	touch_sensor_kit.resetByPowerMode();
	if (touch_sensor_kit.ear_left_touched()) {
		leds::turnOnEarLeft(RGB::pure_blue);
	} else {
		leds::turnOffEarLeft();
	}
	if (touch_sensor_kit.ear_right_touched()) {
		leds::turnOnEarRight(RGB::pure_blue);
	} else {
		leds::turnOffEarRight();
	}
	if (touch_sensor_kit.belt_right_front_touched()) {
		leds::turnOnBeltRightBack(RGB::pure_blue);
	} else {
		leds::turnOffBeltRightBack();
	}
	if (touch_sensor_kit.belt_right_back_touched()) {
		leds::turnOnBeltRightFront(RGB::pure_blue);
	} else {
		leds::turnOffBeltRightFront();
	}
	if (touch_sensor_kit.belt_left_front_touched()) {
		leds::turnOnBeltLeftBack(RGB::pure_blue);
	} else {
		leds::turnOffBeltLeftBack();
	}
	if (touch_sensor_kit.belt_left_back_touched()) {
		leds::turnOnBeltLeftFront(RGB::pure_blue);
	} else {
		leds::turnOffBeltLeftFront();
	}
	rtos::ThisThread::sleep_for(500ms);
}

using namespace leds;
void update_touched_colors()
{
	if (touch_sensor_kit.ear_left_touched() && random_numbers[0] == 0) {
		turnOffEarLeft();
		random_numbers[0] = 3;
	}
	if (touch_sensor_kit.ear_right_touched() && random_numbers[1] == 0) {
		turnOffEarRight();
		random_numbers[1] = 3;
	}
	if (touch_sensor_kit.belt_right_back_touched() && random_numbers[2] == 0) {
		turnOffBeltRightBack();
		random_numbers[2] = 3;
	}
	if (touch_sensor_kit.belt_right_front_touched() && random_numbers[3] == 0) {
		turnOffBeltRightFront();
		random_numbers[3] = 3;
	}
	if (touch_sensor_kit.belt_left_back_touched() && random_numbers[4] == 0) {
		turnOffBeltLeftBack();
		random_numbers[4] = 3;
	}
	if (touch_sensor_kit.belt_left_front_touched() && random_numbers[5] == 0) {
		turnOffBeltLeftFront();
		random_numbers[5] = 3;
	}
}
auto are_all_blue_touched() -> bool
{
	for (auto number: random_numbers) {
		if (number == 0) {
			return false;
		}
	}
	return true;
}

void catch_colors()
{
	leds::setLedsWithRandomColors();
	rtos::ThisThread::sleep_for(500ms);
	while (!(are_all_blue_touched())) {
		touch_sensor_kit.updateState();
		// touch_sensor_kit.resetByPowerMode();
		update_touched_colors();
		rtos::ThisThread::sleep_for(10ms);
	}
	touch_sensor_kit.resetByPowerMode();
	rtos::ThisThread::sleep_for(1ms);
	log_info("animation::rainbow");
	ledkit.start(&LedKit::animation::rainbow);
	rtos::ThisThread::sleep_for(5s);
	ledkit.stop();
	rtos::ThisThread::sleep_for(2s);
}

void design_pattern(uint8_t sequence)
{
	auto defined_sequence = uint8_t {};

	if (sequence == 0 || sequence == 0xFF) {
		defined_sequence = static_cast<uint8_t>(std::rand() / ((RAND_MAX + 1U) / 254));
	} else {
		defined_sequence = sequence;
	}

	bool run = true;

	start_animation();

	launch_sequence(defined_sequence);

	start_animation();

	auto digit = uint8_t {0};
	for (uint8_t i(0); i < 8; i++) {
		digit = 1 & (defined_sequence >> i);
		touch_sensor_kit.resetByPowerMode();
		rtos::ThisThread::sleep_for(500ms);
		do {
			touch_sensor_kit.updateState();
			// touch_sensor_kit.resetByPowerMode();
			rtos::ThisThread::sleep_for(500ms);
		} while (!touch_sensor_kit.ear_left_touched() && !touch_sensor_kit.ear_right_touched());

		if (((touch_sensor_kit.ear_left_touched() && !touch_sensor_kit.ear_right_touched()) && digit == 1) ||
			((touch_sensor_kit.ear_right_touched() && !touch_sensor_kit.ear_left_touched()) && digit == 0)) {
			leds::ears.setColorAtIndex(digit, RGB::pure_green);
			leds::ears.show();
			rtos::ThisThread::sleep_for(1s);
			leds::ears.setColorAtIndex(digit, RGB::black);
			leds::ears.show();
			rtos::ThisThread::sleep_for(1s);
		} else {
			// log_info("Failed !\n\n");
			failed_animation();
			run = false;
			break;
		}
	}

	if (run) {
		win_animation();
	}
	rtos::ThisThread::sleep_for(2s);
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	touch_sensor_kit.setup();
	ledkit.init();

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));

	// touch_sensor_kit.calibration();
	touch_sensor_kit.adjust_sensivity(0x00);
	leds::turnOffAllLeds();
	rtos::ThisThread::sleep_for(2s);
	while (true) {
		// update_leds();
		// design_pattern(0);
		catch_colors();
		rtos::ThisThread::sleep_for(1ms);
	}
}
