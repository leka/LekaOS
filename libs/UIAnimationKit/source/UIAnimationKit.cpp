// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "UIAnimationKit.h"

using namespace leka;
using namespace std::chrono;

UIAnimationKit::UIAnimationKit(rtos::Thread &thread, events::EventQueue &event_queue)
	: _thread(thread), _event_queue(event_queue)
{
	_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void UIAnimationKit::start(interface::CGAnimation &animation)
{
	stop();

	_animation = &animation;
	_animation->start();

	_animation_queue_id = _event_queue.call_every(25ms, _animation, &interface::CGAnimation::run);
}

void UIAnimationKit::stop()
{
	_event_queue.cancel(_animation_queue_id);

	if (_animation != nullptr) {
		_animation->stop();
	}
}
