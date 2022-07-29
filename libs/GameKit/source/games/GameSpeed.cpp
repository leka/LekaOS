

#include "games/GameSpeed.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;
namespace {
void do_nothing(leka::Position position) {}
}	// namespace

void GameSpeed::start()
{
	choseRandomCaptorAsTarget();

	_startTime				 = rtos::Kernel::Clock::now();
	auto update_touched_func = [&](Position position) { updateTouchedColor(position); };
	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(update_touched_func);

	_touch_sensor_kit.start();
	_running = true;
}

void GameSpeed::run()
{
	if (_score == _scoreToWinLevel) {
		_touch_sensor_kit.registerOnSensorTouched(do_nothing);
		_ledManager.playReinforcer();

		_running = false;

	} else if (_win || !checkForTimeout()) {
		choseRandomCaptorAsTarget();
		_startTime = rtos::Kernel::Clock::now();
	}
}

void GameSpeed::stop()
{
	_running = false;
	_score	 = 0;
	_ledManager.stop();
}

auto GameSpeed::isRunning() const -> bool
{
	return _running;
}

auto GameSpeed::checkForTimeout() -> bool
{
	auto roundDuration		  = static_cast<int>((rtos::Kernel::Clock::now() - _startTime).count());
	auto time_still_available = roundDuration < _maximumDuration;
	if (!time_still_available) {
		_score = 0;
	}

	return time_still_available;
}

void GameSpeed::choseRandomCaptorAsTarget()
{
	uint8_t targetNumber = leka::utils::math::random8(0, 5);
	setTarget(targetNumber);
	_ledManager.turnOffAllComponents();
	_ledManager.turnOnComponent(_target, leka::RGB::pure_blue);
	_win = false;
}

void GameSpeed::setTarget(uint8_t targetNumber)
{
	switch (targetNumber) {
		case 0:
			_target = Position::ear_left;
			break;
		case 1:
			_target = Position::ear_right;
			break;
		case 2:
			_target = Position::belt_right_back;
			break;
		case 3:
			_target = Position::belt_right_front;
			break;
		case 4:
			_target = Position::belt_left_back;
			break;
		case 5:
			_target = Position::belt_left_front;
			break;
		default:
			break;
	}
}
void GameSpeed::updateTouchedColor(Position component)
{
	constexpr RGB win_color	 = RGB::pure_green;
	constexpr RGB lose_color = RGB::pure_red;

	_win = (component == _target);

	if (_win) {
		_ledManager.turnOnComponent(component, win_color);
		++_score;

	} else {
		_ledManager.turnOnComponent(component, lose_color);
	}
}
