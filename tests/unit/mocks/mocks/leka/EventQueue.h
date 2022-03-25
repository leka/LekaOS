// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "equeue_stub.h"
#include "gmock/gmock.h"

namespace leka::mock {

class EventQueue
{
  public:
	EventQueue()
	{
		equeue_stub.void_ptr			= &ptr;
		equeue_stub.call_cb_immediately = true;
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
