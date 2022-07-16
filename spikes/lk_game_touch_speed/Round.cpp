
#pragma once

#include "Round.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LedManager.h"
#include "LogKit.h"
#include "MathUtils.h"
#include "Position.h"

using namespace leka;
namespace leds {

Round::Round(LedManager &ledManager, leka::TouchSensorKit &touchSensorKit)
	: _ledManager(ledManager), _touch_sensor_kit(touchSensorKit)
{
}

void Round::choseRandomCaptorAsTarget()
{
	log_info("dans choseRandomCaptor");
	rtos::ThisThread::sleep_for(500ms);
	uint8_t targetNumber = leka::utils::math::random8(0, 5);
	associate_target(targetNumber);

	_ledManager.setRandomLedGroupWithColor(_target);
}
void Round::setColorWinIfWon(Position touched)
{
	constexpr RGB win_color = RGB::pure_green;

	if (touched == _target) {
		_ledManager.turnOnLedGroup(touched, win_color);
		log_info("met win à true");
		_win = true;
		rtos::ThisThread::sleep_for(500ms);
	}
}
void Round::setColorLoseIfLose(Position touched)
{
	constexpr RGB lose_color = RGB::pure_red;

	if (touched != _target) {
		_ledManager.turnOnLedGroup(touched, lose_color);
		log_info("met win à false");
		_win = false;
		rtos::ThisThread::sleep_for(500ms);
	}
}

void Round::associate_target(uint8_t targetNumber)
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
void Round::update_touched_colors(Position component)
{
	log_info("dans update_touched_colors");
	rtos::ThisThread::sleep_for(500ms);
	setColorWinIfWon(component);
	setColorLoseIfLose(component);
}

auto Round::is_target_touched() const -> bool
{
	return _win;
}

void Round::updateTouchedPosition()
{
	log_info("dans setTouched");
	rtos::ThisThread::sleep_for(500ms);

	if (_touch_sensor_kit.component_touched(Position::ear_left)) {
		_onTouch(Position::ear_left);
	}
	if (_touch_sensor_kit.component_touched(Position::ear_right)) {
		_onTouch(Position::ear_right);
	}

	if (_touch_sensor_kit.component_touched(Position::belt_back_right)) {
		_onTouch(Position::belt_back_right);
	}

	if (_touch_sensor_kit.component_touched(Position::belt_front_right)) {
		_onTouch(Position::belt_front_right);
	}
	if (_touch_sensor_kit.component_touched(Position::belt_back_left)) {
		_onTouch(Position::belt_back_left);
	}

	if (_touch_sensor_kit.component_touched(Position::belt_back_right)) {
		_onTouch(Position::belt_front_left);
	}
}
void Round::resetFlag()
{
	_touched_flag = 1;
}
void Round::setFlag()
{
	_touched_flag = _touched_flag * 2;
	log_debug("_touched_flag = %i ", _touched_flag);
}

auto Round::getFlag() const -> uint8_t
{
	return _touched_flag;
}

}	// namespace leds
