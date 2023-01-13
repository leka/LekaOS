// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/LL.h"

namespace leka::mock {

class CoreLLMock : public interface::LL
{
  public:
	CoreLLMock();
	virtual ~CoreLLMock();

	MOCK_METHOD(void, rawMemoryWrite, (uintptr_t destination, uint32_t data), (override));
	MOCK_METHOD(void, rawMemoryWrite, (uintptr_t destination, uint8_t data), (override));
};

}	// namespace leka::mock
