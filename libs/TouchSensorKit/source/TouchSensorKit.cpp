// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::init()
{
	for (Sensor sensor: _sensors) {
		sensor.component.init();
	}
}

void TouchSensorKit::start()
{
	for (Sensor sensor: _sensors) {
		sensor.component.read();
	}
}

void TouchSensorKit::run()
{
	for (Sensor sensor: _sensors) {
		sensor.component.read();
	}
}
void TouchSensorKit::stop()
{
	for (Sensor sensor: _sensors) {
		sensor.component.read();
	}
}

auto TouchSensorKit::isTouched(Position position) -> bool
{
	const auto pos = static_cast<uint8_t>(position);
	for (Sensor sensor: _sensors) {
		if (pos == sensor.position) {
			auto lastState	  = _sensor.state;
			auto currentState = (_sensor.read());
			if (!lastState && currentState) {
				if (_on_sensor_touched_callback != nullptr) {
					_on_sensor_touched_callback(position);
				}
				return true
			}
		}
	}
	return false;
}

auto TouchSensorKit::isReleased(Position position) -> bool
{
	const auto pos = static_cast<uint8_t>(position);
	for (Sensor sensor: _sensors) {
		if (pos == sensor.position) {
			auto lastState	  = _sensor.state;
			auto currentState = (_sensor.read());
			if (lastState && !currentState) {
				if (_on_sensor_touched_callback != nullptr) {
					_on_sensor_touched_callback(position);
				}
				return true
			}
		}
	}
	return false;
}

void TouchSensorKit::calibrate(Position position) {}

void TouchSensorKit::read(uint8_t position)
{
	for (Sensor sensor: _sensors) {
		if (pos == sensor.position) {
			_sensors.state = _sensors.component.read();
		}
	}
}

void TouchSensorKit::reset(uint8_t position)
{
	for (Sensor sensor: _sensors) {
		if (pos == sensor.position) {
			_sensors.state = _sensors.component.reset();
		}
	}
}

void TouchSensorKit::setSensitivity(uint8_t position, uint16_t value, bool saved)
{
	for (Sensor sensor: _sensors) {
		if (pos == sensor.position) {
			_sensors.state = _sensors.component.setSensitivity(value, saved);
		}
	}
}

void TouchSensorKit::registerOnSensorTouched(std::function<void(Position &)> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}

void TouchSensorKit::registerOnSensorReleased(std::function<void(Position &)> const &on_sensor_released_callback)
{
	_on_sensor_released_callback = on_sensor_released_callback;
}
