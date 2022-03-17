// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/HighResClock.h"
#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "ColorKit.h"
#include "LEDAnimation.h"

namespace leka {

class LedKit
{
  public:
	static constexpr auto kNumberOfLedsEars = 2;
	static constexpr auto kNumberOfLedsBelt = 20;

	LedKit(rtos::Thread &thread, events::EventQueue &event_queue, const CoreLED<kNumberOfLedsEars> &ears,
		   const CoreLED<kNumberOfLedsBelt> &belt)
		: _thread(thread), _event_queue(event_queue), _ears {ears}, _belt {belt}
	{
		_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
	};

	void start(interface::LEDAnimation *animation);
	void stop();

  private:
	rtos::Thread &_thread;
	events::EventQueue &_event_queue;

	CoreLED<kNumberOfLedsEars> _ears;
	CoreLED<kNumberOfLedsBelt> _belt;

	interface::LEDAnimation *_animation = nullptr;

	int _animation_queue_id {};
};

}	// namespace leka
