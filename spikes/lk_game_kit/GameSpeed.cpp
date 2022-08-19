

#include "GameSpeed.h"

using namespace leka;

void GameSpeed::start()
{
	choseRandomCaptorAsTarget();

	auto update_touched_func = [this](Position position) { updateTouchedColor(position); };
	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(update_touched_func);

	_touch_sensor_kit.start();
	_running = true;
}

void GameSpeed::run()
{
	if (_target_touched || _step == _step_max) {
		_target_touched = false;
		choseRandomCaptorAsTarget();
		_step = 0;
	} else if (_score == _scoreToWinLevel) {
		_touch_sensor_kit.registerOnSensorTouched([](Position position) {
			// do nothing
		});
		_led_manager.playReinforcer();
		_step	 = 0;
		_running = false;
	} else {
		++_step;
	}
}

void GameSpeed::stop()
{
	_running = false;
	_score	 = 0;
	_led_manager.stop();
}

auto GameSpeed::isRunning() const -> bool
{
	return _running;
}

void GameSpeed::choseRandomCaptorAsTarget()
{
	uint8_t targetNumber = leka::utils::math::random8(0, 5);
	setTarget(targetNumber);
	_led_manager.turnOffAllComponents();
	_led_manager.turnOnComponent(_target, leka::RGB::pure_blue);
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
void GameSpeed::updateTouchedColor(Position component_touched)
{
	constexpr RGB win_color	 = RGB::pure_green;
	constexpr RGB lose_color = RGB::pure_red;

	_target_touched = (component_touched == _target);

	if (_target_touched) {
		_led_manager.turnOnComponent(component_touched, win_color);
		++_score;

	} else {
		_led_manager.turnOnComponent(component_touched, lose_color);
	}
}
