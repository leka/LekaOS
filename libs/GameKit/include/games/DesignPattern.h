// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "Game.h"
#include "LedKit.h"
#include "LedManager.h"
#include "RGB.h"
#include "TouchSensorKit.h"

namespace leka {
class DesignPattern : public interface::Game
{
  public:
	DesignPattern(leds::LedManager &led_manager, TouchSensorKit &touch_sensor_kit)
		: _led_manager(led_manager), _touch_sensor_kit(touch_sensor_kit) {};
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	void isTouched(const Position position);
	void isReleased(const Position position);

	leds::LedManager &_led_manager;
	TouchSensorKit &_touch_sensor_kit;

	static constexpr size_t _level = 4;
	std::array<Position, _level> _pattern {};
	uint8_t _iterator {};

	bool _running;

	std::array<bool, TouchSensorKit::kNumberOfSensors> _state;
	RGB _start_color {RGB::pure_blue};
	RGB _win_color {RGB::pure_green};

	enum class Step
	{
		LAUNCH,
		PLAY,
		WIN
	};

	Step _step;
};
}	// namespace leka
