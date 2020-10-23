/**
 * @file LekaScreen.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "PinNames.h"

#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LekaLCD.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

  private:
	mbed::PwmOut _brightness;
	LekaLCD _lcd;
};

#endif
