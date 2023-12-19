// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/Thread.h"

#include "CoreEventFlags.h"
#include "interface/libs/EventLoop.h"

namespace leka::stub {

class EventLoopKit : public interface::EventLoop
{
  public:
	EventLoopKit() = default;

	void registerCallback(const callback_t &callback) final;

	void start(bool loop_once = true) final;
	void stop() final;
	void exit() final;

	void loop() final;

	void spy_setNumberOfLoops(int i);

  private:
	bool must_exit = false;

	int loops				= 0;
	int spy_number_of_loops = 1;

	callback_t _callback {};
};

}	// namespace leka::stub
