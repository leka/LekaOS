
#pragma once

#include <cstdint>

#include "drivers/HighResClock.h"

#include "Game.h"
#include "LedKit.h"
#include "LedManager.h"
#include "MathUtils.h"
#include "Position.h"
#include "RGB.h"
#include "TouchSensorKit.h"

enum class GameState
{
	PlayingGame,
	TouchedTarget,
	PlayingAnimation,
	AnimationPlayed
};

class GameTouchColor : public leka::interface::Game
{
  public:
	GameTouchColor(leds::LedManager &ledManager, leka::TouchSensorKit &touchSensorKit)
		: _ledManager(ledManager), _touch_sensor_kit(touchSensorKit)

	{
	}
	void start() override;
	void run() override;
	void stop() override;
	[[nodiscard]] auto isRunning() const -> bool override;

  private:
	leds::LedManager &_ledManager;
	leka::TouchSensorKit &_touch_sensor_kit;

	uint8_t _score {0};
	bool _running = false;
	uint8_t _level {0};
	leka::Position _target {};
	GameState _state = GameState::PlayingGame;
	bool _keeplaying = false;
	std::array<uint8_t, 6> _random_numbers {};

	void setLedsWithRandomColors();
	void updateTouchedColor(leka::Position component);
	auto are_all_blue_touched() -> bool;
};
