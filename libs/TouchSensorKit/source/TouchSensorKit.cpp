// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"
#include <array>

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::initialize()
{
	_ear_left.init();
	_ear_right.init();
	_belt_left_back.init();
	_belt_left_front.init();
	_belt_right_back.init();
	_belt_right_front.init();

	setSensitivity(Position::ear_left, default_max_sensitivity_value);
	setSensitivity(Position::ear_right, default_max_sensitivity_value);
	setSensitivity(Position::belt_left_back, default_max_sensitivity_value);
	setSensitivity(Position::belt_left_front, default_max_sensitivity_value);
	setSensitivity(Position::belt_right_back, default_max_sensitivity_value);
	setSensitivity(Position::belt_right_front, default_max_sensitivity_value);

	_event_queue.dispatch_forever();
}

void TouchSensorKit::setRefreshDelay(std::chrono::milliseconds delay)
{
	_refresh_delay = delay;
}

void TouchSensorKit::enable()
{
	disable();
	_event_id = _event_queue.call_every(_refresh_delay, [this] { run(); });
}

void TouchSensorKit::disable()
{
	_event_queue.cancel(_event_id);
}

void TouchSensorKit::run()
{
	auto constexpr positions =
		std::to_array<Position>({Position::ear_left, Position::ear_right, Position::belt_left_back,
								 Position::belt_left_front, Position::belt_right_back, Position::belt_right_front});

	for (Position position: positions) {
		auto is_touched = isTouched(position);
		if (is_touched && !_previous_is_touched[position] && _on_sensor_touched_callback != nullptr) {
			_on_sensor_touched_callback(position);
		}
		if (!is_touched && _previous_is_touched[position] && _on_sensor_released_callback != nullptr) {
			_on_sensor_released_callback(position);
		}
		_previous_is_touched[position] = is_touched;
	}
}

void TouchSensorKit::registerOnSensorTouched(std::function<void(const Position)> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}

void TouchSensorKit::registerOnSensorReleased(std::function<void(const Position)> const &on_sensor_released_callback)
{
	_on_sensor_released_callback = on_sensor_released_callback;
}

auto TouchSensorKit::isTouched(Position position) -> bool
{
	auto read = bool {};
	switch (position) {
		case Position::ear_left:
			read = _ear_left.read();
			break;
		case Position::ear_right:
			read = _ear_right.read();
			break;
		case Position::belt_left_back:
			read = _belt_left_back.read();
			break;
		case Position::belt_left_front:
			read = _belt_left_front.read();
			break;
		case Position::belt_right_back:
			read = _belt_right_back.read();
			break;
		case Position::belt_right_front:
			read = _belt_right_front.read();
			break;
		default:
			break;
	}
	return read;
}

void TouchSensorKit::setSensitivity(Position position, float value)
{
	switch (position) {
		case Position::ear_left:
			_ear_left.setSensitivity(value);
			break;
		case Position::ear_right:
			_ear_right.setSensitivity(value);
			break;
		case Position::belt_left_back:
			_belt_left_back.setSensitivity(value);
			break;
		case Position::belt_left_front:
			_belt_left_front.setSensitivity(value);
			break;
		case Position::belt_right_back:
			_belt_right_back.setSensitivity(value);
			break;
		case Position::belt_right_front:
			_belt_right_front.setSensitivity(value);
			break;
		default:
			break;
	}
}
