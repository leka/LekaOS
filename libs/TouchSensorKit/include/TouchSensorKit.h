// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
#include "interface/drivers/TouchSensor.h"
#include "internal/TouchSensorSystem.h"

namespace leka {

enum class Position
{
	ear_left,
	ear_right,
	belt_left_back,
	belt_left_front,
	belt_right_back,
	belt_right_front
};

inline const auto positions = std::array<Position, 6> {
	Position::ear_left,		   Position::ear_right,		  Position::belt_left_back,
	Position::belt_left_front, Position::belt_right_back, Position::belt_right_front,
};

class TouchSensorKit
{
  public:
	static constexpr auto kNumberOfSensors = uint8_t {6};

	explicit TouchSensorKit(std::array<interface::TouchSensor, kNumberOfSensors> &sensors) : _sensors(sensors) {};

	void init();
	void start();
	void run();
	void stop();

	auto isTouched(Position position) -> bool;
	auto isReleased(Position position) -> bool;

	void calibrate(Position position);

	void registerOnSensorTouched(std::function<void(Position &)> const &on_sensor_touched_callback);
	void registerOnSensorReleased(std::function<void(Position &)> const &on_sensor_released_callback);

  private:
	void read(uint8_t position);
	void reset(uint8_t position);
	void setSensitivity(uint8_t position, uint16_t value, bool saved = false);

	std::array<interface::TouchSensor, kNumberOfSensors> &_sensors;

	std::function<void(Position &)> _on_sensor_touched_callback {};
	std::function<void(Position &)> _on_sensor_released_callback {};
};
}	// namespace leka
