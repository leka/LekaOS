#pragma once

#include <cstdint>

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

	[[nodiscard]] auto getTarget() const -> uint8_t { return _target; }

	void setColorWinIfWon(uint8_t touched);
	void setColorLoseIfLoose(uint8_t touched);
	void setRandomCaptorWithColor();
	void choseRandomCaptorAsTarget();
	void update_touched_colors();
	[[nodiscard]] auto is_target_touched() const -> bool;
	void setTouched();
	void setFlag();
	void resetFlag();
	[[nodiscard]] auto getFlag() const -> uint8_t;

  private:
	LedManager &_ledManager;
	leka::TouchSensorKit &_touch_sensor_kit;
	uint8_t _target;
	bool _win = false;
	uint8_t _touched;
	uint8_t _touched_flag = 1;
};

}	// namespace leds
