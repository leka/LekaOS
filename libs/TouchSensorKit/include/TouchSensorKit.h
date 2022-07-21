// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreTouchSensor.h"
#include "LogKit.h"
#include "internal/TouchSensorSystem.h"

namespace leka {

enum class Position
{
	ear_left,
	ear_right,
	belt_left_back,
	belt_left_front,
	belt_right_back,
	belt_right_front,
};

inline const auto positions = std::array<Position, 6> {
	Position::ear_left,		   Position::ear_right,		  Position::belt_left_back,
	Position::belt_left_front, Position::belt_right_back, Position::belt_right_front,
};

class TouchSensorKit
{
  public:
	explicit TouchSensorKit() = default;
	void init();

	[[nodiscard]] auto isTouched(Position position) -> bool;
	[[nodiscard]] auto isTouchedAny() -> bool;

	void setPowerMode(Position position, PowerMode power_mode);
	void resetByPowerMode();

	void setSensitivity(Position position, uint16_t value, bool saved = false);

	void registerOnSensorTouched(std::function<void()> const &on_sensor_touched_callback);

	static constexpr uint16_t default_max_sensitivity_value {0x0FFF};
	static constexpr uint16_t default_min_sensitivity_value {0x0000};

  private:
	std::array<CoreTouchSensor, 6> _sensors = {sensor_ear_left,		   sensor_ear_right,	   sensor_belt_left_back,
											   sensor_belt_left_front, sensor_belt_right_back, sensor_belt_right_front};

	// struct State{} _state;
	std::function<void()> _on_sensor_touched_callback {};
};
}	// namespace leka
