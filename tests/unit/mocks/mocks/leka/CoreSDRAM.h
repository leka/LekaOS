// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/SDRAM.hpp"

namespace leka::mock {

class CoreSDRAM : public interface::SDRAM
{
  public:
	CoreSDRAM();
	virtual ~CoreSDRAM();

	MOCK_METHOD(void, setupSDRAMConfig, (), (override));
	MOCK_METHOD(FMC_SDRAM_TimingTypeDef, setupTimingConfig, (), (override));
	MOCK_METHOD(DMA_HandleTypeDef, setupDMA, (), (override));

	MOCK_METHOD(uint8_t, initialize, (), (override));
	MOCK_METHOD(void, initializeController, (), (override));
	MOCK_METHOD(void, initializationSequence, (), (override));
};

}	// namespace leka::mock
