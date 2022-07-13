
#pragma once

#include <cstdint>

#include "LedKit.h"
#include "LedManager.h"
#include "RGB.h"
#include "Round.h"

namespace leds {

class Level
{
  public:
	static const uint8_t timeEasiestLevel = 60000;
	static const uint8_t scoreToWinLevel  = 5;
	Level(LedManager &ledManager, uint8_t number, Round &currentRound, leka::TouchSensorKit &touchSensorKit)
		: _ledManager(ledManager),
		  _currentRound(currentRound),
		  _maximumDuration(timeEasiestLevel - 1000 * number),
		  _touch_sensor_kit(touchSensorKit)
	{
	}

	void playAllRounds();
	[[nodiscard]] auto returnScore() const -> uint8_t { return _score; }

  private:
	LedManager &_ledManager;
	Round &_currentRound;
	leka::TouchSensorKit &_touch_sensor_kit;
	uint8_t _maximumDuration;
	uint8_t _score {};

	void updateScore();
	auto playOnce() -> bool;
	auto nextRound() -> bool;
};

}	// namespace leds
