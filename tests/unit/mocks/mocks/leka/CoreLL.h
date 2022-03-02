// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreLL.h"
#include "gmock/gmock.h"

namespace leka::mock {

class CoreLLMock : public CoreLL
{
  public:
	MOCK_METHOD(void, rawMemoryWrite, (uintptr_t destination, uint32_t data), (override));
};

}	// namespace leka::mock
