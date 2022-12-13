// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "interface/drivers/LL.h"

namespace leka {

class CoreLL : public interface::LL
{
  public:
	void rawMemoryWrite(uintptr_t destination, uint32_t data) final;
	void rawMemoryWrite(uintptr_t destination, uint8_t data) final;
};

}	// namespace leka
