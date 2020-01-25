/**
 * @file HelloWorld.cpp
 * @author Ladislas de Toldi (ladislas [at] detoldi.me)
 *
 * @version 1.0
 * @date 2019-09-09
 *
 * @copyright Copyright (c) 2019
 */

#include "mbed.h"
#include "HelloWorld.h"

HelloWorld::HelloWorld(void) {
	// nothing to do
}

/**
 * @brief Start the 3 threads to blink the leds
 */
void HelloWorld::start(void) {

	thread1.start(callback(blink, &led1Data));
	thread2.start(callback(blink, &led2Data));
	thread3.start(callback(blink, &led3Data));

}	// void start(void)

/**
 * @brief Blink a led for a specified amount of time
 *
 * @param data Data structure with led pin & sleep time
 */
void HelloWorld::blink(Data *data) {

	while (true) {
		data->led = !data->led;
		rtos::ThisThread::sleep_for(data->sleepTime);
	}

}	// void blink(Data * data)
