// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::init()
{
	_event_loop.registerCallback([this] { run(); });
	_ear_left.init();
	_ear_right.init();
	_belt_left_back.init();
	_belt_left_front.init();
	_belt_right_back.init();
	_belt_right_front.init();
}

void TouchSensorKit::start()
{
	stop();
	_event_loop.start();
}

void TouchSensorKit::run()
{
	auto states		   = std::array<bool, kNumberOfSensors> {};
	auto previousState = bool {};
	auto currentState  = bool {};
	while (true) {
		for (Position position: _positions) {
			currentState = readAtPosition(position);

			if (!previousState && currentState && _on_sensor_touched_callback != nullptr) {
				_on_sensor_touched_callback(position);
			}
			if (previousState && !currentState && _on_sensor_released_callback != nullptr) {
				_on_sensor_released_callback(position);
			}

			previousState = currentState;
		}
		rtos::ThisThread::sleep_for(100ms);
	}
}
void TouchSensorKit::stop()
{
	_event_loop.stop();
}

void TouchSensorKit::registerOnSensorTouched(std::function<void(Position)> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}

void TouchSensorKit::registerOnSensorReleased(std::function<void(Position)> const &on_sensor_released_callback)
{
	_on_sensor_released_callback = on_sensor_released_callback;
}

void TouchSensorKit::updateStateAtPosition(Position position)
{
	switch (position) {
		case Position::ear_left:
			_state.at(static_cast<size_t>(position)) = _ear_left.read();
			break;
		case Position::ear_right:
			_state.at(static_cast<size_t>(position)) = _ear_right.read();
			break;
		case Position::belt_left_back:
			_state.at(static_cast<size_t>(position)) = _belt_left_back.read();
			break;
		case Position::belt_left_front:
			_state.at(static_cast<size_t>(position)) = _belt_left_front.read();
			break;
		case Position::belt_right_back:
			_state.at(static_cast<size_t>(position)) = _belt_right_back.read();
			break;
		case Position::belt_right_front:
			_state.at(static_cast<size_t>(position)) = _belt_right_front.read();
			break;
	}
}

auto TouchSensorKit::geAtPosition(Position position) -> bool
{
	return _state.at(static_cast<size_t>(position));
}

void TouchSensorKit::resetAtPosition(Position position)
{
	switch (position) {
		case Position::ear_left:
			_ear_left.reset();
			break;
		case Position::ear_right:
			_ear_right.reset();
			break;
		case Position::belt_left_back:
			_belt_left_back.reset();
			break;
		case Position::belt_left_front:
			_belt_left_front.reset();
			break;
		case Position::belt_right_back:
			_belt_right_back.reset();
			break;
		case Position::belt_right_front:
			_belt_right_front.reset();
			break;
		default:
			break;
	}
}

void TouchSensorKit::setSensitivityAtPosition(Position position, uint16_t value, bool saved)
{
	switch (position) {
		case Position::ear_left:
			_ear_left.setSensitivity(value, saved);
			break;
		case Position::ear_right:
			_ear_right.setSensitivity(value, saved);
			break;
		case Position::belt_left_back:
			_belt_left_back.setSensitivity(value, saved);
			break;
		case Position::belt_left_front:
			_belt_left_front.setSensitivity(value, saved);
			break;
		case Position::belt_right_back:
			_belt_right_back.setSensitivity(value, saved);
			break;
		case Position::belt_right_front:
			_belt_right_front.setSensitivity(value, saved);
			break;
		default:
			break;
	}
}
