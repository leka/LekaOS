// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
#include "external/TouchSensorSystem.h"
#include "interface/drivers/TouchSensor.h"
#include "interface/libs/EventLoop.h"

namespace leka {

enum class Position : uint8_t
{
	ear_left		 = position::ear_left,
	ear_right		 = position::ear_right,
	belt_left_back	 = position::belt_left_back,
	belt_left_front	 = position::belt_left_front,
	belt_right_back	 = position::belt_right_back,
	belt_right_front = position::belt_right_front,
};

class TouchSensorKit
{
  public:
	static constexpr auto kNumberOfSensors = uint8_t {6};

	explicit TouchSensorKit(interface::EventLoop &event_loop,
							std::array<interface::TouchSensor *, kNumberOfSensors> sensors)
		: _event_loop(event_loop), _sensors(sensors) {};

	void init();
	void start();
	void run();
	void stop();

	void updateState(Position position);

	void calibrate(Position position);

	void registerOnSensorTouched(std::function<void(Position &)> const &on_sensor_touched_callback);
	void registerOnSensorReleased(std::function<void(Position &)> const &on_sensor_released_callback);

  private:
	void initTouch(Position position);
	auto read(Position position) -> bool;
	void reset(Position position);
	void setSensitivity(Position position, uint16_t value, bool saved = false);

	static constexpr std::array<Position, kNumberOfSensors> _positions {
		Position::ear_left,		   Position::ear_right,		  Position::belt_left_back,
		Position::belt_left_front, Position::belt_right_back, Position::belt_right_front,
	};

	interface::EventLoop &_event_loop;

	std::array<interface::TouchSensor *, kNumberOfSensors> _sensors;
	std::array<bool, kNumberOfSensors> _state {};

	std::function<void(Position &)> _on_sensor_touched_callback {};
	std::function<void(Position &)> _on_sensor_released_callback {};
};
}	// namespace leka
