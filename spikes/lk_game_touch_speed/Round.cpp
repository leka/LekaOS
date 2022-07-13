#include "Round.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LedManager.h"
#include "LogKit.h"
#include "Random8.h"

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
	_target = random8(0, 5);
	_ledManager.setRandomCaptorWithColor(_target);
}
void Round::setColorWinIfWon(uint8_t touched)
{
	constexpr RGB win_color = RGB::pure_green;
	// uint8_t est forcément supérieur à 0 => cause de warning
	if (touched > 5 || touched < 0) {
		return;
	} else if (touched == _target) {
		_ledManager.turnOnCaptor(touched, win_color);
		log_info("met win à true");
		_win = true;
		rtos::ThisThread::sleep_for(500ms);
	}
}
void Round::setColorLoseIfLoose(uint8_t touched)
{
	constexpr RGB lose_color = RGB::pure_red;
	// uint8_t est forcément supérieur à 0 => cause de warning
	if (touched > 5 || touched < 0) {
		return;
	} else if (touched != _target) {
		_ledManager.turnOnCaptor(touched, lose_color);
		log_info("met win à false");
		_win = false;
		rtos::ThisThread::sleep_for(500ms);
	}
}

void Round::update_touched_colors()
{
	log_info("dans update_touched_colors");
	rtos::ThisThread::sleep_for(500ms);
	setColorWinIfWon(_touched);
	setColorLoseIfLoose(_touched);
}

auto Round::is_target_touched() const -> bool
{
	return _win;
}

void Round::setTouched()
{
	log_info("dans setTouched");
	rtos::ThisThread::sleep_for(500ms);

	if (_touch_sensor_kit.ear_left_touched()) {
		_touched = 0;
	}
	if (_touch_sensor_kit.ear_right_touched()) {
		_touched = 1;
	}

	if (_touch_sensor_kit.belt_right_back_touched()) {
		_touched = 2;
	}

	if (_touch_sensor_kit.belt_right_front_touched()) {
		_touched = 3;
	}
	if (_touch_sensor_kit.belt_left_back_touched()) {
		_touched = 4;
	}

	if (_touch_sensor_kit.belt_left_front_touched()) {
		_touched = 5;
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
