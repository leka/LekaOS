
#pragma once

#include <cstdint>

#include "LedKit.h"
#include "LedManager.h"
#include "RGB.h"
#include "Round.h"

class Level
{
  public:
	Level(leds::LedManager &ledManager, uint8_t number, leds::Round &playingRound, leka::TouchSensorKit &touchSensorKit)
		: _ledManager(ledManager),
		  _maximumDuration(_timeEasiestLevel - 1000 * number),
		  _touch_sensor_kit(touchSensorKit),
		  _playingRound(playingRound)
	{
	}

	void playAllRounds();
	[[nodiscard]] auto returnScore() const -> uint8_t { return _score; }

  private:
	static const uint8_t _timeEasiestLevel = 60000;
	static const uint8_t _scoreToWinLevel  = 5;
	leds::LedManager &_ledManager;
	leds::Round &_playingRound;
	leka::TouchSensorKit &_touch_sensor_kit;
	uint8_t _maximumDuration;
	uint8_t _score {};

	// void updateScore();
	auto playOnce() -> bool;
	// auto nextRound() -> bool;
};
