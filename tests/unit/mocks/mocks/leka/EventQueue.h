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
	EventQueue();
	virtual ~EventQueue();

  private:
	struct equeue_event ptr;
};

}	// namespace leka::mock
