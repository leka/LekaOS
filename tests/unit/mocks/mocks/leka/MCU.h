// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/MCU.h"

namespace leka::mock {

class MCU : public interface::MCU
{
  public:
	MOCK_METHOD(MCUID, getID, (), (override));
};

}	// namespace leka::mock
