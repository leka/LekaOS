

#include "games/GameSpeed.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void GameSpeed::start()
{
	registerCallback([this](Position component) { updateTouchedColor(component); });
	_running = true;
}

void GameSpeed::run()
{
	if ((_score < _scoreToWinLevel) && _running) {
		auto round_won = playOnce();
		if (round_won) {
			++_score;

		} else {
			_score = 0;
		}
	}
	if ((_score == _scoreToWinLevel) && _running) {
		_ledManager.playReinforcer();
		_touch_sensor_kit.printState();
		_running = false;
		_score	 = 0;
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

auto GameSpeed::playOnce() -> bool
{
	choseRandomCaptorAsTarget();

	auto start		  = rtos::Kernel::Clock::now();
	int roundDuration = 0;

	auto keep_playing = [&]() {
		auto target_not_touched	  = !isTargetTouched();
		auto time_still_available = roundDuration < _maximumDuration;

		log_debug(_running ? "running true" : "running false");
		log_debug(time_still_available ? "time true" : "time false");
		log_debug("roundDuration : %d", roundDuration);
		return target_not_touched && time_still_available && _running;
	};
	resetFlag();
	while (keep_playing()) {
		uint8_t touched_flags = getFlag();
		_touch_sensor_kit.fakeUpdateState(touched_flags);
		updateTouchedPosition();
		setFlag();
		roundDuration = static_cast<int>((rtos::Kernel::Clock::now() - start).count());
	}

	return isTargetTouched();
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
			_target = Position::belt_back_right;
			break;
		case 3:
			_target = Position::belt_front_right;
			break;
		case 4:
			_target = Position::belt_back_left;
			break;
		case 5:
			_target = Position::belt_front_left;
			break;
		default:
			break;
	}
}
void GameSpeed::updateTouchedColor(Position component)
{
	constexpr RGB win_color	 = RGB::pure_green;
	constexpr RGB lose_color = RGB::pure_red;
	log_debug("touched: %d", component);
	_win = (component == _target);
	if (_win) {
		_ledManager.turnOnComponent(component, win_color);
	} else {
		_ledManager.turnOnComponent(component, lose_color);
	}
	// rtos::ThisThread::sleep_for(500ms);
}

auto GameSpeed::isTargetTouched() const -> bool
{
	return _win;
}

void GameSpeed::updateTouchedPosition() const
{
	log_debug("target: %d", _target);
	if (_touch_sensor_kit.isSensorTouched(Position::ear_left)) {
		_onTouch(Position::ear_left);
	}
	if (_touch_sensor_kit.isSensorTouched(Position::ear_right)) {
		_onTouch(Position::ear_right);
	}

	if (_touch_sensor_kit.isSensorTouched(Position::belt_back_right)) {
		_onTouch(Position::belt_back_right);
	}

	if (_touch_sensor_kit.isSensorTouched(Position::belt_front_right)) {
		_onTouch(Position::belt_front_right);
	}
	if (_touch_sensor_kit.isSensorTouched(Position::belt_back_left)) {
		_onTouch(Position::belt_back_left);
	}

	if (_touch_sensor_kit.isSensorTouched(Position::belt_front_left)) {
		_onTouch(Position::belt_front_left);
	}
}
void GameSpeed::resetFlag()
{
	_touched_flag = 1;
}
void GameSpeed::setFlag()
{
	_touched_flag = _touched_flag * 2;
}

auto GameSpeed::getFlag() const -> uint8_t
{
	return _touched_flag;
}
