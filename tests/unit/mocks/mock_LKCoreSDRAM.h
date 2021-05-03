// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SDRAM_MOCK_H_
#define _LEKA_OS_SDRAM_MOCK_H_

#include "LKCoreSDRAMBase.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreSDRAMMock : public LKCoreSDRAMBase
{
  public:
	MOCK_METHOD(void, setupSDRAMConfig, (), (override));
	MOCK_METHOD(FMC_SDRAM_TimingTypeDef, setupTimingConfig, (), (override));
	MOCK_METHOD(DMA_HandleTypeDef, setupDMA, (), (override));

	MOCK_METHOD(uint8_t, initialize, (), (override));
	MOCK_METHOD(void, initializeController, (), (override));
	MOCK_METHOD(void, initializationSequence, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_SDRAM_MOCK_H_
