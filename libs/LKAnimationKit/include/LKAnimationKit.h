// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LKANIMATIONKIT_H_
#define _LEKA_OS_LIB_LKANIMATIONKIT_H_

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CGAnimation.h"

namespace leka {

class LKAnimationKit
{
  public:
	LKAnimationKit(rtos::Thread &thread, events::EventQueue &event_queue, animation::interface::CGAnimation &animation);

	void start(animation::interface::CGAnimation &animation);
	void stop();

  private:
	rtos::Thread &_thread;
	events::EventQueue &_event_queue;
	animation::interface::CGAnimation &_animation;

	int _animation_id;
	std::chrono::milliseconds _refresh_rate = std::chrono::milliseconds(1);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LKANIMATIONKIT_H_
