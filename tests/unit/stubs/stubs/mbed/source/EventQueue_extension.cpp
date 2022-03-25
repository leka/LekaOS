// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../EventQueue_extension.h"

#include "events/EventQueue.h"

std::function<void()> leka::spy_EventQueue_on_dispatch_forever_call;

void events::EventQueue::dispatch_forever()
{
	if (leka::spy_EventQueue_on_dispatch_forever_call) {
		leka::spy_EventQueue_on_dispatch_forever_call();
	}
}
