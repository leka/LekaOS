/ Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

	using namespace leka;
using namespace std::chrono;

void SwapColor::start()
{
	for (uint8_t i = 0; i < _colors.size(); i++) {
		_iterators.at(i) = _start_color_iterator;
	}

	auto isTouched_func	 = [&](const Position position) { isTouched(position); };
	auto isReleased_func = [&](const Position position) { isReleased(position); };

	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(isTouched_func);
	_touch_sensor_kit.registerOnSensorReleased(isReleased_func);
	_touch_sensor_kit.start();

	_running = true;

	rtos::ThisThread::sleep_for(1s);
}

void SwapColor::run()
{
	if () }

void SwapColor::stop()
{
	_running = false;
	_led_manager.stop();
	_touch_sensor_kit.stop();
}

auto SwapColor::isRunning() const -> bool
{
	return _running;
}

void SwapColor::isTouched(const Position position)
{
	_led_manager.turnOnComponent(position, *_iterators.at(static_cast<size_t>(position)));
	if (_iterators.at(static_cast<size_t>(position)) == _end_color_iterator) {
		_iterators.at(static_cast<size_t>(position)) = _start_color_iterator;
	} else {
		++_iterators.at(static_cast<size_t>(position));
	}
}
