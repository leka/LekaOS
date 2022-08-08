// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SwapColor.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void SwapColor::start()
{
	for (auto &_colors_iterator: _colors_iterators) {
		_colors_iterator = _start_color_iterator;
	}

	auto swap_func = [&](const Position position) { swap(position); };

	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(swap_func);
	_touch_sensor_kit.start();

	_running = true;

	rtos::ThisThread::sleep_for(1s);
}

void SwapColor::run()
{
	rtos::ThisThread::sleep_for(1ms);
}

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

void SwapColor::swap(const Position position)
{
	_led_manager.turnOnComponent(position, *_colors_iterators.at(static_cast<size_t>(position)));
	if (_colors_iterators.at(static_cast<size_t>(position)) == _end_color_iterator) {
		_colors_iterators.at(static_cast<size_t>(position)) = _start_color_iterator;
	} else {
		++_colors_iterators.at(static_cast<size_t>(position));
	}
}
