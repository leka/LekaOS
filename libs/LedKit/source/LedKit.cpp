// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LedKit.h"

using namespace leka;
using namespace std::chrono;

void LedKit::start(interface::LEDAnimation &animation)
{
	stop();

	_animation = &animation;
	_animation->start();

	_animation_queue_id = _event_queue.call_every(40ms, _animation, &interface::LEDAnimation::run);
}

void LedKit::stop()
{
	_event_queue.cancel(_animation_queue_id);

	if (_animation != nullptr) {
		_animation->stop();
	}
}
