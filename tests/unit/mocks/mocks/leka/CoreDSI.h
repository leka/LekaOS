// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/DSI.hpp"

namespace leka::mock {

class CoreDSI : public interface::DSIBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, reset, (), (override));
	MOCK_METHOD(DSI_VidCfgTypeDef, getConfig, (), (override));
	MOCK_METHOD(void, write, (std::span<const uint8_t> data), (override));
};

}	// namespace leka::mock
