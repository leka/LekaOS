// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka {

class CriticalSection
{
  public:
	CriticalSection() = default;

	static void lock();
	static void unlock();
};

}	// namespace leka
