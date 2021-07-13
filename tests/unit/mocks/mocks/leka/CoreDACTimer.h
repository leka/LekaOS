// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DACTIMER_MOCK_H_
#define _LEKA_OS_DACTIMER_MOCK_H_

#include "gmock/gmock.h"
#include "interface/drivers/DacTimer.h"

namespace leka {

class CoreDACTimerMock : public interface::DacTimer
{
  public:
	MOCK_METHOD(void, initialize, (float frequency), (override));
	MOCK_METHOD(void, deInitialize, (), (override));
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, stop, (), (override));

	MOCK_METHOD(TIM_HandleTypeDef, getHandle, (), (override));

	MOCK_METHOD(void, _registerMspCallbacks, (), (override));
	MOCK_METHOD(void, _mspInitCallback, (), (override));   // Can stay private => Test output after init
	MOCK_METHOD(void, _mspDeInitCallback, (), (override));
	MOCK_METHOD(uint32_t, _calculatePeriod, (float frequency), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DACTIMER_MOCK_H_
