// Mbed CMake Template
// Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_HELLOWORLD_H_
#define _LEKA_OS_LIB_HELLOWORLD_H_

#include "chrono"
#include "mbed.h"

///
/// @class HelloWorld
/// @brief This class is used to for debugging purpose to make sure the program is still running
///
class HelloWorld
{
  public:
	///
	/// @brief Struct used to hold led pin & sleep time information
	///
	struct Data {
		DigitalOut led;
		chrono::milliseconds sleepTime;
	};

	HelloWorld(void);
	~HelloWorld(void) {};

	void start(void);

	char const *world = "Hello, Mbed CMake Template!";

  private:
	LowPowerTicker flipper1;
	LowPowerTicker flipper2;
	LowPowerTicker flipper3;

	struct Data led1Data = {DigitalOut(LED1), 500ms};
	struct Data led2Data = {DigitalOut(LED2), 1000ms};
	struct Data led3Data = {DigitalOut(LED3), 1500ms};

	static void blink(Data *data);
};	 // class HelloWorld

#endif	 // _LEKA_OS_LIB_HELLOWORLD_H_
