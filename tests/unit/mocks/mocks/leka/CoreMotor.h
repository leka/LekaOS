// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/Motor.h"

namespace leka::mock {

class CoreMotor : public interface::Motor
{
  public:
	MOCK_METHOD(void, spin, (rotation_t, float), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock
