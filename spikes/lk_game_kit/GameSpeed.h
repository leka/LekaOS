
#pragma once

#include <cstdint>

#include "Game.h"
#include "LedKit.h"
#include "LedManager.h"
#include "Position.h"
#include "RGB.h"
#include "TouchSensorKit.h"

class GameSpeed : public leka::interface::Game
{
  public:
	GameSpeed(leds::LedManager &led_manager, leka::TouchSensorKit &touch_sensor_kit, uint8_t stage_number)
		: _led_manager(led_manager),
		  _touch_sensor_kit(touch_sensor_kit),
		  _step_max(_step_max_level0 - 20 * stage_number)

	{
	}
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	leds::LedManager &_led_manager;
	leka::TouchSensorKit &_touch_sensor_kit;
	leka::Position _target {};

	uint8_t _step_max;
	uint8_t _score {0};
	bool _target_touched			   = false;
	bool _running					   = false;
	uint8_t _step					   = 0;
	static const auto _scoreToWinLevel = 5;
	static const auto _step_max_level0 = 120;

	void choseRandomCaptorAsTarget();
	void updateTouchedColor(leka::Position component);
	void setTarget(uint8_t targetNumber);
};
