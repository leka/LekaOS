// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_EVENT_QUEUE_H_
#define _LEKA_OS_INTERFACE_DRIVER_EVENT_QUEUE_H_

#include <functional>

namespace leka::interface {

class EventQueue
{
  public:
	virtual ~EventQueue() = default;

	virtual void dispatch_forever() = 0;

	virtual void call(std::function<void()> const &f) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_EVENT_QUEUE_H_
