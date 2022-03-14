// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class MCU
{
  public:
	struct MCUID {
		uint32_t front;
		uint32_t middle;
		uint32_t back;
	};

	virtual ~MCU() = default;

	virtual auto getID() -> MCUID = 0;
};

}	// namespace leka::interface
