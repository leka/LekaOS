// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CGAnimation.h"

namespace leka {

class UIAnimationKit
{
  public:
	UIAnimationKit(rtos::Thread &thread, events::EventQueue &event_queue);

	void start(interface::CGAnimation &animation);
	void stop();

  private:
	rtos::Thread &_thread;
	events::EventQueue &_event_queue;
	interface::CGAnimation *_animation = nullptr;

	int _animation_queue_id;
};

}	// namespace leka
