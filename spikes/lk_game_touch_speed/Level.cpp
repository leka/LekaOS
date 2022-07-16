#include "Level.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LedManager.h"
#include "LogKit.h"

using namespace leka;
namespace leds {

auto Level::playOnce() -> bool
{
	rtos::ThisThread::sleep_for(500ms);
	if (_score > scoreToWinLevel || _score < 0) {
		_score = 0;
	}

	_currentRound.choseRandomCaptorAsTarget();

	auto start			= rtos::Kernel::Clock::now();
	int sinceRoundBegin = 0;
	_currentRound.registerCallback([this](Position component) { _currentRound.update_touched_colors(component); });
	do {
		rtos::ThisThread::sleep_for(500ms);
		rtos::ThisThread::sleep_for(1s);
		uint8_t touched_flags = _currentRound.getFlag();
		_touch_sensor_kit.updateState();
		_touch_sensor_kit.resetByPowerMode();
		_currentRound.updateTouchedPosition();
		rtos::ThisThread::sleep_for(1s);
		_currentRound.setFlag();
		sinceRoundBegin = static_cast<int>((rtos::Kernel::Clock::now() - start).count());
	} while (!(_currentRound.is_target_touched()) && (sinceRoundBegin > _maximumDuration));

	_currentRound.resetFlag();

	return _currentRound.is_target_touched();
}

void Level::playAllRounds()
{
	log_info("dans playLevel");
	rtos::ThisThread::sleep_for(500ms);

	while (_score < scoreToWinLevel) {
		if (playOnce()) {
			++_score;
			log_info("score =, %i", _score);
		} else {
			_score = 0;
		}
	}

	_ledManager.playReinforcer();

	_touch_sensor_kit.printState();
	log_info("\n\n");
}
}	// namespace leds
