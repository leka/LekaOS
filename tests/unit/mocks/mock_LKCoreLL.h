// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LL_MOCK_H_
#define _LEKA_OS_LL_MOCK_H_

#include "gmock/gmock.h"

namespace leka {

class LKCoreLLMock : public LKCoreLL
{
  public:
	MOCK_METHOD(void, rawMemoryWrite, (uintptr_t destination, uint32_t data), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_LL_MOCK_H_
