// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/STM32HalBasicTimer.h"

namespace leka::mock {

class STM32HalTimer : public interface::STM32HalBasicTimer
{
  public:
	MOCK_METHOD(TIM_HandleTypeDef &, getHandle, (), (override));

	MOCK_METHOD(void, linkDACTimer, (DAC_ChannelConfTypeDef *), (override));

	MOCK_METHOD(void, initialize, (uint32_t), (override));
	MOCK_METHOD(void, terminate, (), (override));

	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock
