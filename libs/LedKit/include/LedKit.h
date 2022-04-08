// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/HighResClock.h"
#include "rtos/Thread.h"

#include "ColorKit.h"
#include "LEDAnimation.h"
#include "animations/BlinkGreen.h"
#include "animations/Fire.h"
#include "animations/Rainbow.h"
#include "animations/Sleeping.h"
#include "animations/SpinBlink.h"
#include "animations/Sprinkles.h"
#include "interface/drivers/EventFlags.h"

namespace leka {

class LedKit
{
  public:
	static constexpr auto kNumberOfLedsEars = 2;
	static constexpr auto kNumberOfLedsBelt = 20;

	LedKit(rtos::Thread &thread, interface::EventFlags &event_flags, const CoreLED<kNumberOfLedsEars> &ears,
		   const CoreLED<kNumberOfLedsBelt> &belt)
		: _thread(thread), _event_flags(event_flags), _ears(ears), _belt(belt) {};

	void init();
	void start(interface::LEDAnimation *animation);
	void run();
	void stop();

	void initializeAnimation();
	void runAnimation();

	struct animation {
		static inline auto sleeping	   = led::animation::Sleeping {};
		static inline auto blink_green = led::animation::BlinkGreen {};
		static inline auto spin_blink  = led::animation::SpinBlink {};
		static inline auto fire		   = led::animation::Fire {};
		static inline auto sprinkles   = led::animation::Sprinkles {};
		static inline auto rainbow	   = led::animation::Rainbow {};
	};

	struct flags {
		static constexpr uint32_t START_LED_ANIMATION_FLAG = (1UL << 1);
		static constexpr uint32_t STOP_LED_ANIMATION_FLAG  = (1UL << 2);
	};

  private:
	rtos::Thread &_thread;
	interface::EventFlags &_event_flags;

	CoreLED<kNumberOfLedsEars> _ears;
	CoreLED<kNumberOfLedsBelt> _belt;

	interface::LEDAnimation *_animation = nullptr;
};

}	// namespace leka
