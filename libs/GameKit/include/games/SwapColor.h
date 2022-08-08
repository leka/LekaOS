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
class SwapColor : public interface::Game
{
  public:
	SwapColor(leds::LedManager &led_manager, TouchSensorKit &touch_sensor_kit)
		: _led_manager(led_manager), _touch_sensor_kit(touch_sensor_kit) {};
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	void swap(Position position);

	leds::LedManager &_led_manager;
	TouchSensorKit &_touch_sensor_kit;

	bool _running;

	static constexpr uint8_t kNumberOfColors {8};
	std::array<RGB, kNumberOfColors> _colors {RGB::white,	  RGB::black,  RGB::pure_red, RGB::pure_green,
											  RGB::pure_blue, RGB::yellow, RGB::cyan,	  RGB::magenta};

	std::array<RGB *, TouchSensorKit::kNumberOfSensors> _colors_iterators;
	RGB *_start_color_iterator {_colors.data()};
	RGB *_end_color_iterator {_colors.data() + _colors.size()};
};

}	// namespace leka
