// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
namespace leka::interface {

class EventLoop
{
  public:
	using callback_t = std::function<void(void)>;

	virtual ~EventLoop() = default;

	virtual void registerCallback(const callback_t &) = 0;

	virtual void start() = 0;
	virtual void stop()	 = 0;
	virtual void exit()	 = 0;

	virtual void loop() = 0;
};

}	// namespace leka::interface
