// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::init()
{
	for (Position position: _positions) {
		init(position);
	}
	_event_loop.registerCallback([this] { run(); });
}

void TouchSensorKit::start()
{
	stop();
	_event_loop.start();
}

void TouchSensorKit::run()
{
	for (Position position: _positions) {
		updateState(position);
	}
	rtos::ThisThread::sleep_for(100ms);
}
void TouchSensorKit::stop()
{
	_event_loop.stop();
}

void TouchSensorKit::updateState(Position position)
{
	const auto index = static_cast<size_t>(position);

	auto previousState = bool {};
	auto currentState  = bool {};

	previousState	 = _state.at(index);
	_state.at(index) = read(position);
	currentState	 = _state.at(index);

	if (!previousState && currentState && _on_sensor_touched_callback != nullptr) {
		_on_sensor_touched_callback(position);
	}
	if (previousState && !currentState && _on_sensor_released_callback != nullptr) {
		_on_sensor_released_callback(position);
	}
}

void TouchSensorKit::calibrate(Position position)
{
	auto value = uint16_t {CoreTouchSensor::default_min_sensitivity_value};
	auto step  = uint16_t {10};

	constexpr auto accurate_read_count = uint8_t {10};
	auto read_count					   = uint8_t {0};

	setSensitivity(position, value);

	while (read_count < accurate_read_count && value <= CoreTouchSensor::default_max_sensitivity_value) {
		if (read(position)) {
			read_count = 0;
			value += step;
			setSensitivity(position, value);
			log_info(" Value : %d\n\n", value);

		} else {
			read_count++;
		}
		rtos::ThisThread::sleep_for(100ms);
	}
	setSensitivity(position, value, true);
}

void TouchSensorKit::registerOnSensorTouched(std::function<void(Position &)> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}

void TouchSensorKit::registerOnSensorReleased(std::function<void(Position &)> const &on_sensor_released_callback)
{
	_on_sensor_released_callback = on_sensor_released_callback;
}

void TouchSensorKit::init(Position position)
{
	const auto index = static_cast<size_t>(position);

	_sensors.at(index)->init();
}

auto TouchSensorKit::read(Position position) -> bool
{
	const auto index = static_cast<size_t>(position);

	return _sensors.at(index)->read();
}

void TouchSensorKit::reset(Position position)
{
	const auto index = static_cast<size_t>(position);

	_sensors.at(index)->reset();
}

void TouchSensorKit::setSensitivity(Position position, uint16_t value, bool saved)
{
	const auto index = static_cast<size_t>(position);

	_sensors.at(index)->setSensitivity(value, saved);
}
