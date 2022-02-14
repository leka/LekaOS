// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_
#define _LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "interface/drivers/EventQueue.h"

namespace leka {

class CoreEventQueue : public interface::EventQueue
{
  public:
	explicit CoreEventQueue() = default;

	void dispatch_forever() final;

	void call(auto f, auto... params) { _event_queue.call(f, params...); }

	// ? Overload needed for mbed::BLE compatibility
	void callMbedCallback(mbed::Callback<void()> const &f);

  private:
	rtos::Thread _event_queue_thread {};
	events::EventQueue _event_queue {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_
