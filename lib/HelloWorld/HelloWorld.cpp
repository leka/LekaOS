// Mbed CMake Template
// Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
// SPDX-License-Identifier: Apache-2.0

#include "HelloWorld.h"

HelloWorld::HelloWorld(void)
{
	// nothing to do
}

///
/// @brief Start the 3 threads to blink the leds
///
void HelloWorld::start(void)
{
	flipper1.attach(callback(blink, &led1Data), led1Data.sleepTime);
	flipper2.attach(callback(blink, &led2Data), led2Data.sleepTime);
	flipper3.attach(callback(blink, &led3Data), led3Data.sleepTime);
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
