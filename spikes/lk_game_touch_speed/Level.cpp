#include "Level.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LedManager.h"

using namespace leka;

auto Level::playOnce() -> bool
{
	if (_score > _scoreToWinLevel || _score < 0) {
		_score = 0;
	}

	_playingRound.choseRandomCaptorAsTarget();

	auto start		  = rtos::Kernel::Clock::now();
	int roundDuration = 0;
	_playingRound.registerCallback([this](Position component) { _playingRound.updateTouchedColor(component); });

	auto keep_playing = [&]() {
		auto target_not_touched	  = !_playingRound.isTargetTouched();
		auto time_still_available = roundDuration > _maximumDuration;

		auto ret = target_not_touched && time_still_available;

		return ret;
	};

	do {
		uint8_t touched_flags = _playingRound.getFlag();
		_touch_sensor_kit.updateState();
		_touch_sensor_kit.resetByPowerMode();
		_playingRound.updateTouchedPosition();
		_playingRound.setFlag();
		roundDuration = static_cast<int>((rtos::Kernel::Clock::now() - start).count());

	} while (keep_playing());

	_playingRound.resetFlag();

	return _playingRound.isTargetTouched();
}

void Level::playAllRounds()
{
	while (_score < _scoreToWinLevel) {
		auto round_won = playOnce();
		if (round_won) {
			++_score;

		} else {
			_score = 0;
		}
	}

	_ledManager.playReinforcer();

	_touch_sensor_kit.printState();
}
