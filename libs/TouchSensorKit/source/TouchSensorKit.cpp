// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"
#include <array>

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

	_ear_left.setSensitivity(default_max_sensitivity_value, true);
	_ear_right.setSensitivity(default_max_sensitivity_value, true);
	_belt_left_back.setSensitivity(default_max_sensitivity_value, true);
	_belt_left_front.setSensitivity(default_max_sensitivity_value, true);
	_belt_right_back.setSensitivity(default_max_sensitivity_value, true);
	_belt_right_front.setSensitivity(default_max_sensitivity_value, true);
}

void TouchSensorKit::start()
{
	stop();
	_event_loop.start();
}

void TouchSensorKit::run()
{
	auto const positions =
		std::array<Position, 6> {Position::ear_left,		Position::ear_right,	   Position::belt_left_back,
								 Position::belt_left_front, Position::belt_right_back, Position::belt_right_front};
	auto state = std::array<bool, kNumberOfSensors> {};

	while (true) {
		for (Position position: positions) {
			auto previousState						= state.at(static_cast<size_t>(position));
			auto currentState						= readAtPosition(position);
			state.at(static_cast<size_t>(position)) = currentState;

			if (!previousState && currentState && _on_sensor_touched_callback != nullptr) {
				_on_sensor_touched_callback(position);
			}
			if (previousState && !currentState && _on_sensor_released_callback != nullptr) {
				_on_sensor_released_callback(position);
			}
		}
		rtos::ThisThread::sleep_for(100ms);
	}
}
void TouchSensorKit::stop()
{
	_event_loop.stop();
}

void TouchSensorKit::registerOnSensorTouched(std::function<void(const Position)> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}

void TouchSensorKit::registerOnSensorReleased(std::function<void(const Position)> const &on_sensor_released_callback)
{
	_on_sensor_released_callback = on_sensor_released_callback;
}

auto TouchSensorKit::readAtPosition(Position position) -> bool
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
