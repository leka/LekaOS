// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SCREENSAVER_H_
#define _LEKA_OS_LIB_SCREENSAVER_H_

#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "DrawingTools.h"

namespace leka {

class Screensaver
{
  public:
	Screensaver();

	void start();
	void stop();

  private:
	static void squareBouncing();

	static bool in_run;
	rtos::Thread *screensaver_thread;
};

}	// namespace leka

#endif
