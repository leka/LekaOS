// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "equeue_stub.h"
#include "gmock/gmock.h"
#include "stubs/leka/CoreEventQueue.h"
#include "stubs/mbed/EventQueue_extension.h"

namespace leka::mock {

class EventQueue
{
  public:
	EventQueue()
	{
		equeue_stub.void_ptr = &ptr;

		spy_CoreEventQueue_on_dispatch_forever_call = [] { equeue_stub.call_cb_immediately = true; };
		spy_EventQueue_on_dispatch_forever_call		= [] { equeue_stub.call_cb_immediately = true; };
	}

	~EventQueue()
	{
		equeue_stub.void_ptr			= nullptr;
		equeue_stub.call_cb_immediately = false;
	}

  private:
	struct equeue_event ptr;
};

}	// namespace leka::mock
