// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/Mutex.h"

namespace leka {

class CoreMutex
{
  public:
	CoreMutex() = default;

	void lock();
	void unlock();
	auto try_lock() -> bool;

  private:
	rtos::Mutex _mutex {};
};

}	// namespace leka
