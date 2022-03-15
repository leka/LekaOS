// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/MCU.h"

namespace leka {

class CoreMCU : public interface::MCU
{
  public:
	explicit CoreMCU() = default;

	[[nodiscard]] auto getID() -> MCUID final;

  private:
	const uintptr_t id_address {0x1FF0F420};
};

}	// namespace leka
