// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LTDC_MOCK_H_
#define _LEKA_OS_LTDC_MOCK_H_

#include "gmock/gmock.h"
#include "interface/CoreLTDC.h"

namespace leka {

class CoreLTDCMock : public interface::CoreLTDC
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_LTDC_MOCK_H_
