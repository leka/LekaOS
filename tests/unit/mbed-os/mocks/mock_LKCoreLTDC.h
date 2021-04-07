// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LTDC_MOCK_H_
#define _LEKA_OS_LTDC_MOCK_H_

#include "LKCoreLTDCBase.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreLTDCMock : public LKCoreLTDCBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, setupLayerConfig, (), (override));
	MOCK_METHOD(void, setupTimingConfig, (), (override));
	MOCK_METHOD(void, setupBackgroundConfig, (), (override));
	MOCK_METHOD(void, configurePeriphClock, (), (override));
	MOCK_METHOD(void, configureLayer, (), (override));
	MOCK_METHOD(LTDC_HandleTypeDef, getHandle, (), (override));
	MOCK_METHOD(LTDC_LayerCfgTypeDef, getLayerConfig, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_LTDC_MOCK_H_
