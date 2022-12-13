// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class LL
{
  public:
	virtual ~LL() = default;

	virtual void rawMemoryWrite(uintptr_t destination, uint32_t data) = 0;
	virtual void rawMemoryWrite(uintptr_t destination, uint8_t data)  = 0;
};

}	// namespace leka::interface
