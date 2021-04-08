// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LKANIMATIONKIT_H_
#define _LEKA_OS_LIB_LKANIMATIONKIT_H_

#include "rtos/EventFlags.h"
#include "rtos/Thread.h"

#include "CGAnimation.h"

namespace leka {

class LKAnimationKit
{
  public:
	LKAnimationKit(rtos::Thread &thread, interface::CGAnimation &animation);

	void start(interface::CGAnimation &animation);
	void stop();

  private:
	void runner();

  private:
	rtos::Thread &_thread;
	interface::CGAnimation &_animation;

	rtos::EventFlags _event_flags;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LKANIMATIONKIT_H_
