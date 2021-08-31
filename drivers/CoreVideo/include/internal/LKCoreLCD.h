// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LCD_H_
#define _LEKA_OS_DRIVER_LCD_H_

#include "drivers/PwmOut.h"

#include "LKCoreLCDBase.h"
#include "LKCoreLCDDriverBase.h"

namespace leka {

class LKCoreLCD : public LKCoreLCDBase
{
  public:
	explicit LKCoreLCD(LKCoreLCDDriverBase &driver);

	void initialize() final;

	void turnOn() final;
	void turnOff() final;

	void setBrightness(float value) final;

  private:
	LKCoreLCDDriverBase &_driver;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LCD_H_
