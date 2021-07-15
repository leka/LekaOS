// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_LCD_SOURCE_DRIVER_BASE_H_
#define _LEKA_OS_DRIVER_LK_CORE_LCD_SOURCE_DRIVER_BASE_H_

namespace leka::interface {

class LKCoreLCDDriver
{
  public:
	~LKCoreLCDDriver() = default;

	virtual void initialize()			   = 0;
	virtual void setLandscapeOrientation() = 0;

	virtual void turnOn()  = 0;
	virtual void turnOff() = 0;

	virtual void setBrightness(float value) = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LK_CORE_LCD_SOURCE_DRIVER_BASE_H_
