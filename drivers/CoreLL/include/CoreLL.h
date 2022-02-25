// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

class CoreLL
{
  public:
	virtual void rawMemoryWrite(uintptr_t destination, uint32_t data)
	{
		// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
		*reinterpret_cast<uintptr_t *>(destination) = data;
	}
};

}	// namespace leka
