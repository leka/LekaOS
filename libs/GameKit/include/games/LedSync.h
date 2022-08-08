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
class LedSync : public interface::Game
{
  public:
	LedSync(leds::LedManager &led_manager, TouchSensorKit &touch_sensor_kit)
		: _led_manager(led_manager), _touch_sensor_kit(touch_sensor_kit) {};
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	void turnOn(const Position position);
	void turnOff(const Position position);

	leds::LedManager &_led_manager;
	TouchSensorKit &_touch_sensor_kit;

	bool _running;

	RGB _color {RGB::pure_blue};
};
}	// namespace leka
