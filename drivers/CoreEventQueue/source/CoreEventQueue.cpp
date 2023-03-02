// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreEventQueue.h"

using namespace leka;

void CoreEventQueue::dispatch_forever()
{
	_event_queue_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}
