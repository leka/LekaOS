// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

class CoreLL
{
  public:
	virtual ~CoreLL() = default;

	virtual void rawMemoryWrite(uintptr_t destination, uint32_t data)
	{
		// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
		auto *ptr = reinterpret_cast<uint32_t *>(destination);
		*ptr	  = data;
	}

	virtual void rawMemoryWrite(uintptr_t destination, uint8_t data)
	{
		// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
		auto *ptr = reinterpret_cast<uint8_t *>(destination);
		*ptr	  = data;
	}
};

}	// namespace leka
