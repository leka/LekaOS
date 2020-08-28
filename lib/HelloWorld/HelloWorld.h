/**
 * @file HelloWorld.h
 * @author Ladislas de Toldi (ladislas [at] detoldi.me)
 *
 * @version 1.0
 * @date 2019-09-09
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef _LK_ALPHA_OS_MBED_HELLOWORLD_H_
#define _LK_ALPHA_OS_MBED_HELLOWORLD_H_

#include "mbed.h"
#include "chrono"

/**
 * @class HelloWorld
 * @brief This class is used to for debugging purpose to make sure the program is still running
 */

class HelloWorld {

	/**
	 * @brief Struct used to hold led pin & sleep time information
	 */
	struct Data {
		DigitalOut led;
		chrono::milliseconds sleepTime;
	};

  public:
	HelloWorld(void);
	~HelloWorld(void) {};

	void start(void);

	char const *world = "Hello, World!";

  private:
	Thread thread1;
	Thread thread2;
	Thread thread3;

	struct Data led1Data = {DigitalOut(LED1), 500ms};
	struct Data led2Data = {DigitalOut(LED2), 1000ms};
	struct Data led3Data = {DigitalOut(LED3), 1500ms};

	static void blink(Data *data);

};	 // class HelloWorld

#endif
