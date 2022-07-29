
#pragma once

#include <cstdint>

#include "drivers/HighResClock.h"

#include "Game.h"
#include "LedKit.h"
#include "LedManager.h"
#include "Position.h"
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

	leka::Position _target {};
	bool _win		 = false;
	bool _keeplaying = false;
	rtos::Kernel::Clock::time_point _startTime;

	auto checkForTimeout() -> bool;

	void choseRandomCaptorAsTarget();
	void updateTouchedColor(leka::Position component);

	void setTarget(uint8_t targetNumber);

	// [[nodiscard]] isSensorTouched(Position sensor) const->bool;
	// [[nodiscard]] updateStateCaptor(Position sensor) const->bool;
};
