// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKAnimationKit.h"

using namespace leka;
using namespace animation;

LKAnimationKit::LKAnimationKit(rtos::Thread &thread, events::EventQueue &event_queue, interface::CGAnimation &animation)
	: _thread(thread), _event_queue(event_queue), _animation(animation)
{
	_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void LKAnimationKit::start(interface::CGAnimation &animation)
{
	stop();	  // Avoid speed up of animation

	_animation = animation;
	animation.start();

	_animation_id = _event_queue.call_every(_refresh_rate, &animation, &interface::CGAnimation::run);
}

void LKAnimationKit::stop()
{
	_event_queue.cancel(_animation_id);

	_animation.stop();
}
