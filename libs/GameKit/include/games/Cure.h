// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "CoreLED.h"
#include "Game.h"
#include "RGB.h"
#include "TouchSensorKit.h"

namespace leka {
class Cure : public interface::Game
{
  public:
	Cure(interface::LED &ears, interface::LED &belt, TouchSensorKit &touch_sensor_kit)
		: _ears(ears), _belt(belt), _touch_sensor_kit(touch_sensor_kit) {};
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	void update(Position position);
	void setDots();

	bool _running;

	interface::LED &_ears;
	interface::LED &_belt;
	TouchSensorKit &_touch_sensor_kit;

	bool _touched;

	enum class State
	{
		base,
		sick,
		forbidden,
		cured
	};

	static constexpr uint8_t kNumberOfLedsBelt {20};
	std::array<State, kNumberOfLedsBelt> _dots {};

	uint8_t _mobile_cursor_index {};

	RGB _mobile_cursor_color {RGB::cyan};
	RGB _sick_fixed_dot_color {RGB::pure_red};
	RGB _cured_fixed_dot_color {RGB::pure_green};
	RGB _forbidden_fixed_dot_color {RGB::white};
	RGB _base_fixed_dot_color {RGB::black};

	enum class Step
	{
		START,
		PLAY,
		END
	};

	Step _step;
};

}	// namespace leka
