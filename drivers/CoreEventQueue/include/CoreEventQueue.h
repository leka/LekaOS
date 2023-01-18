// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "interface/drivers/EventQueue.h"

namespace leka {

class CoreEventQueue : public interface::EventQueue
{
  public:
	explicit CoreEventQueue() = default;

	void dispatch_forever() final;

	auto call(auto f, auto... params) -> int { return _event_queue.call(f, params...); }

	auto call_every(std::chrono::duration<int, std::milli> duration, auto f, auto... params) -> int
	{
		return _event_queue.call_every(duration, f, params...);
	}

	void cancel(int id) { _event_queue.cancel(id); }

  private:
	rtos::Thread _event_queue_thread {};
	events::EventQueue _event_queue {};
};

}	// namespace leka
