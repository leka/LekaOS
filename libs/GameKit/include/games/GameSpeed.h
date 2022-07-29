
#pragma once

#include <cstdint>

#include "Game.h"
#include "LedKit.h"
#include "LedManager.h"
#include "RGB.h"
#include "TouchSensorKit.h"

class GameSpeed : public leka::interface::Game
{
  public:
	GameSpeed(leds::LedManager &ledManager, uint8_t number, leka::TouchSensorKit &touchSensorKit)
		: _ledManager(ledManager),
		  _touch_sensor_kit(touchSensorKit),
		  _maximumDuration(_timeEasiestLevel - 1000 * number)

	{
	}
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	static const uint16_t _timeEasiestLevel = 6000;
	static const uint8_t _scoreToWinLevel	= 5;
	leds::LedManager &_ledManager;
	leka::TouchSensorKit &_touch_sensor_kit;
	uint16_t _maximumDuration;
	uint8_t _score {0};
	bool _running = false;
	mbed::Callback<void(leka::Position)> _onTouch;
	leka::Position _target {};
	bool _win			  = false;
	uint8_t _touched_flag = 1;	 //? only on mock purpose

	auto playOnce() -> bool;
	void choseRandomCaptorAsTarget();
	void updateTouchedColor(leka::Position component);
	void updateTouchedPosition() const;
	[[nodiscard]] auto isTargetTouched() const -> bool;
	void setFlag();									 //? only on mock purpose
	void resetFlag();								 //? only on mock purpose
	[[nodiscard]] auto getFlag() const -> uint8_t;	 //?only on mock purpose
	void setTarget(uint8_t targetNumber);
	void registerCallback(mbed::Callback<void(leka::Position)> const &onTouch) { _onTouch = onTouch; }
};
