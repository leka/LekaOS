// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/Thread.h"

#include "CoreEventFlags.h"
#include "interface/libs/EventLoop.h"

namespace leka {

class EventLoopKit : public interface::EventLoop
{
  public:
	EventLoopKit();

	void registerCallback(const callback_t &callback) final;

	void start() final;
	void stop() final;
	void exit() final;

  private:
	struct flag {
		static constexpr uint32_t START = (1UL << 1);
	};

	void loop() final;

	bool must_exit = false;

	callback_t _callback {};
	rtos::Thread _thread {};
	CoreEventFlags _flags {};
};

}	// namespace leka
