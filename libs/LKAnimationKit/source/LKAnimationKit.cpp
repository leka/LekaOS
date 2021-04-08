// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKAnimationKit.h"

#include "rtos/ThisThread.h"

#define RUN_ANIMATION_FLAG (1UL << 0)

using namespace std::chrono;
using namespace leka;

LKAnimationKit::LKAnimationKit(rtos::Thread &thread, interface::CGAnimation &animation)
	: _thread(thread), _animation(animation)
{
	_thread.start({this, &LKAnimationKit::runner});
}

void LKAnimationKit::start(interface::CGAnimation &animation)
{
	stop();

	_animation = animation;
	_animation.start();

	_event_flags.set(RUN_ANIMATION_FLAG);
}

void LKAnimationKit::stop()
{
	_animation.stop();
}

__attribute__((noreturn)) void LKAnimationKit::runner()
{
	while (true) {
		_event_flags.wait_any(RUN_ANIMATION_FLAG);
		_animation.run();
	}
}
