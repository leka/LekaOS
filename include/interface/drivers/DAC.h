// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <span>

#include "interface/drivers/STM32Hal.h"

namespace leka::interface {

class DACBase
{
  public:
	virtual ~DACBase() = default;

	[[nodiscard]] virtual auto getHandle() -> DAC_HandleTypeDef & = 0;

	virtual void initialize() = 0;
	virtual void terminate()  = 0;

	virtual void start() = 0;
	virtual void stop()	 = 0;
};

class DACDMA : public DACBase
{
  public:
	virtual ~DACDMA() = default;

	virtual void registerDataToPlay(std::span<uint16_t> data)							 = 0;
	virtual void registerDMACallbacks(std::function<void()> const &on_half_transfer,
									  std::function<void()> const &on_complete_transfer) = 0;
};

}	// namespace leka::interface
