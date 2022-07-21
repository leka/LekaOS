// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::init()
{
	for (Position position: positions) {
		setPowerMode(position, PowerMode::normal);
	}
}

auto TouchSensorKit::isTouched(Position position) -> bool
{
	const auto pos = static_cast<uint8_t>(position);

	auto read = (1 == _sensors.at(pos).read());
	if (read && _on_sensor_touched_callback != nullptr) {
		_on_sensor_touched_callback();
	}
	return read;
}

auto TouchSensorKit::isTouchedAny() -> bool
{
	for (Position position: positions) {
		if (isTouched(position)) {
			return true;
		};
	};
	return false;
}

void TouchSensorKit::setPowerMode(Position position, PowerMode power_mode)
{
	const auto pos = static_cast<uint8_t>(position);
	_sensors.at(pos).setPowerMode(power_mode);
}

void TouchSensorKit::resetByPowerMode()
{
	for (Position position: positions) {
		if (isTouched(position)) {
			setPowerMode(position, PowerMode::low);
			rtos::ThisThread::sleep_for(20ms);
			setPowerMode(position, PowerMode::normal);
			rtos::ThisThread::sleep_for(20ms);
		}
	}
}

void TouchSensorKit::setSensitivity(Position position, uint16_t value, bool saved)
{
	const auto pos = static_cast<uint8_t>(position);
	_sensors.at(pos).setSensitivity(value, saved);

	rtos::ThisThread::sleep_for(10ms);
}

void TouchSensorKit::registerOnSensorTouched(std::function<void()> const &on_sensor_touched_callback)
{
	_on_sensor_touched_callback = on_sensor_touched_callback;
}
