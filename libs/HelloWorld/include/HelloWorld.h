// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "drivers/DigitalOut.h"
#include "drivers/LowPowerTicker.h"

namespace leka {

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
		mbed::DigitalOut led;
		std::chrono::milliseconds sleepTime;
	};

	HelloWorld(void) = default;

	void start(void);

	char const *world = "Hello, LekaOS!";

  private:
	mbed::LowPowerTicker flipper1;
	struct Data led1Data = {mbed::DigitalOut(LED1), std::chrono::milliseconds(500)};
	static void blink(Data *data);
};	 // class HelloWorld

}	// namespace leka
