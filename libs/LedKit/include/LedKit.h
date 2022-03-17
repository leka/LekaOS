// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/HighResClock.h"
#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "BlinkGreen.h"
#include "ColorKit.h"
#include "Fire.h"
#include "LEDAnimation.h"
#include "LoadingGreen.h"
#include "LoadingOrange.h"
#include "LoadingRed.h"
#include "LoadingYellow.h"
#include "Rainbow.h"
#include "Sleeping.h"
#include "SpinBlink.h"
#include "Sprinkles.h"
#include "Waiting.h"

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

	struct animation {
		static inline auto sleeping		   = led::animation::Sleeping {};
		static inline auto charging_green  = led::animation::LoadingGreen {};
		static inline auto charging_yellow = led::animation::LoadingYellow {};
		static inline auto charging_orange = led::animation::LoadingOrange {};
		static inline auto charging_red	   = led::animation::LoadingRed {};
		static inline auto blink_green	   = led::animation::BlinkGreen {};
		static inline auto spin_blink	   = led::animation::SpinBlink {};
		static inline auto fire			   = led::animation::Fire {};
		static inline auto sprinkles	   = led::animation::Sprinkles {};
		static inline auto rainbow		   = led::animation::Rainbow {};
		static inline auto waiting		   = led::animation::Waiting {};
	};

  private:
	rtos::Thread &_thread;
	events::EventQueue &_event_queue;

	CoreLED<kNumberOfLedsEars> _ears;
	CoreLED<kNumberOfLedsBelt> _belt;

	interface::LEDAnimation *_animation = nullptr;

	int _animation_queue_id {};
};

}	// namespace leka
