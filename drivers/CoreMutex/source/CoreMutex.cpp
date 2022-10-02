// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMutex.h"

using namespace leka;

void CoreMutex::lock()
{
	_mutex.lock();
}

void CoreMutex::unlock()
{
	_mutex.unlock();
}

auto CoreMutex::try_lock() -> bool
{
	return _mutex.trylock();
}
