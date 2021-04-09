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
	MOCK_METHOD(void, configureLayer, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_LTDC_MOCK_H_
