// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

namespace leka::interface {

class EventQueue
{
  public:
	virtual ~EventQueue() = default;

	virtual void dispatch_forever() = 0;

	void call(auto f, auto... params);
};

}	// namespace leka::interface
