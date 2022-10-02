// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/QDAC.h"

namespace leka::mock {

class CoreQDAC : public interface::QDAC
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, write, (uint8_t, uint16_t), (override));
	MOCK_METHOD(uint16_t, read, (uint8_t), (override));
};

}	// namespace leka::mock
