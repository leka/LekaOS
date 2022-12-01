// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/Accelerometer.h"

namespace leka::mock {

class Accelerometer : public interface::Accelerometer
{
  public:
	MOCK_METHOD((std::tuple<float, float, float>), getXYZ, (), (override));
};

}	// namespace leka::mock
