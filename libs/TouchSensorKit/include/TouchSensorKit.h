// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <map>

#include "CoreEventQueue.h"
#include "Position.h"
#include "interface/drivers/TouchSensor.h"

namespace leka {

class TouchSensorKit
{
  public:
	explicit TouchSensorKit(interface::TouchSensor &ear_left, interface::TouchSensor &ear_right,
							interface::TouchSensor &belt_left_back, interface::TouchSensor &belt_left_front,
							interface::TouchSensor &belt_right_back, interface::TouchSensor &belt_right_front)
		: _ear_left(ear_left),
		  _ear_right(ear_right),
		  _belt_left_back(belt_left_back),
		  _belt_left_front(belt_left_front),
		  _belt_right_back(belt_right_back),
		  _belt_right_front(belt_right_front) {};

	void initialize();

	void setRefreshDelay(std::chrono::milliseconds delay);
	void enable();
	void disable();

	void registerOnSensorTouched(std::function<void(const Position)> const &on_sensor_touched_callback);
	void registerOnSensorReleased(std::function<void(const Position)> const &on_sensor_released_callback);

	auto isTouched(Position position) -> bool;

  private:
	void run();

	void setSensitivity(Position position, float value);

	CoreEventQueue _event_queue {};
	std::chrono::milliseconds _refresh_delay {100};
	int _event_id {};

	interface::TouchSensor &_ear_left;
	interface::TouchSensor &_ear_right;
	interface::TouchSensor &_belt_left_back;
	interface::TouchSensor &_belt_left_front;
	interface::TouchSensor &_belt_right_back;
	interface::TouchSensor &_belt_right_front;

	static constexpr auto default_max_sensitivity_value = float {1.F};

	std::map<Position, bool> _previous_is_touched {};

	std::function<void(const Position)> _on_sensor_touched_callback {};
	std::function<void(const Position)> _on_sensor_released_callback {};
};
}	// namespace leka
