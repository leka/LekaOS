#pragma once

#include <cstdint>

#include "platform/Callback.h"

#include "LedKit.h"
#include "LedManager.h"
#include "RGB.h"
#include "TouchSensorKit.h"
#include "interface/drivers/LED.h"

using namespace std::chrono;

namespace leds {

class Round
{
  public:
	Round(LedManager &ledManager, leka::TouchSensorKit &touchSensorKit);

	[[nodiscard]] auto getTarget() const -> leka::Position { return _target; }

	void setColorWinIfWon(leka::Position touched);
	void setColorLoseIfLose(leka::Position touched);
	void setRandomCaptorWithColor();
	void choseRandomCaptorAsTarget();
	void update_touched_colors(leka::Position component);
	[[nodiscard]] auto is_target_touched() const -> bool;
	void updateTouchedPosition();
	void setFlag();
	void resetFlag();
	void associate_target(uint8_t targetNumber);
	[[nodiscard]] auto getFlag() const -> uint8_t;
	void registerCallback(mbed::Callback<void(leka::Position)> onTouch) { _onTouch = onTouch; }

  private:
	LedManager &_ledManager;
	leka::TouchSensorKit &_touch_sensor_kit;
	mbed::Callback<void(leka::Position)> _onTouch;
	leka::Position _target;
	bool _win			  = false;
	uint8_t _touched_flag = 1;
};

}	// namespace leds
