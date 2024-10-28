// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "stm32f7xx_hal.h"

namespace leka::interface {

class DSIBase
{
  public:
	virtual ~DSIBase() = default;

	virtual void initialize() = 0;
	virtual void start()	  = 0;
	virtual void reset()	  = 0;

	virtual auto getConfig() -> DSI_VidCfgTypeDef = 0;

	virtual void write(std::span<const uint8_t> data) = 0;
};

}	// namespace leka::interface
