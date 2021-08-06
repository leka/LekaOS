// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#ifndef _LEKA_OS_CORE_TOUCH_H_
#define _LEKA_OS_CORE_TOUCH_H_

#include "mbed.h"

#include "CoreDACTouch.h"
#include "CoreIOExpander.h"

namespace leka {

class CoreTouch
{
  public:
	CoreTouch(MCP23017 sensors, CoreDACTouch dac_left_sensors, CoreDACTouch dac_right_sensors, mbed::DigitalIn ear_left,
			  mbed::DigitalIn belt_back_left, mbed::DigitalIn belt_front_left, mbed::DigitalIn ear_right,
			  mbed::DigitalIn belt_back_right, mbed::DigitalIn belt_front_right)
		: _sensors(sensors),
		  _dac_left_sensors(dac_left_sensors),
		  _dac_right_sensors(dac_right_sensors),
		  _ear_left(ear_left),
		  _belt_back_left(belt_back_left),
		  _belt_front_left(belt_front_left),
		  _ear_right(ear_right),
		  _belt_back_right(belt_back_right),
		  _belt_front_right(belt_front_right) {};

	void init();

  private:
	void DACInit();

  private:
	MCP23017 &_sensors;
	CoreDACTouch &_dac_left_sensors;
	CoreDACTouch &_dac_right_sensors;

	mbed::interface::DigitalIn &_ear_left;
	mbed::interface::DigitalIn &_belt_back_left;
	mbed::interface::DigitalIn &_belt_front_left;
	mbed::interface::DigitalIn &_ear_right;
	mbed::interface::DigitalIn &_belt_back_right;
	mbed::interface::DigitalIn &_belt_front_right;
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_TOUCH_H_
