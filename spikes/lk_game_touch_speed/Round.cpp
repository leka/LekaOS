
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
	uint8_t targetNumber = leka::utils::math::random8(0, 5);
	setTarget(targetNumber);
	_ledManager.turnOff();
	_ledManager.turnOn(_target, leka::RGB::pure_blue);
}
void Round::setColorTarget(Position touched)
{
	constexpr RGB win_color = RGB::pure_green;

	if (touched == _target) {
		_ledManager.turnOn(touched, win_color);
		_win = true;
		rtos::ThisThread::sleep_for(500ms);
	}

	constexpr RGB lose_color = RGB::pure_red;

	if (touched != _target) {
		_ledManager.turnOn(touched, lose_color);
		_win = false;
		rtos::ThisThread::sleep_for(500ms);
	}
}

void Round::setTarget(uint8_t targetNumber)
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
void Round::updateTouchedColor(Position component)
{
	setColorTarget(component);
}

auto Round::isTargetTouched() const -> bool
{
	return _win;
}

void Round::updateTouchedPosition()
{
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

	if (_touch_sensor_kit.isSensorTouched(Position::belt_back_right)) {
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
}

auto Round::getFlag() const -> uint8_t
{
	// devrait récupérer les positions touchées au sein du touchsensor et on aurait pas besoin de get flag et de set
	// flag
	return _touched_flag;
}

}	// namespace leds
