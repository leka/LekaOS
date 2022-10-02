// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "HelloWorld.h"

using namespace mbed;
using namespace std::chrono;

namespace leka {

///
/// @brief Start the 3 threads to blink the leds
///
void HelloWorld::start(void)
{
	flipper1.attach(callback(blink, &led1Data), led1Data.sleepTime);
}

///
/// @brief Blink a led for a specified amount of time
///
/// @param data Data structure with led pin & sleep time
///
void HelloWorld::blink(Data *data)
{
	data->led = !data->led;
}

}	// namespace leka
