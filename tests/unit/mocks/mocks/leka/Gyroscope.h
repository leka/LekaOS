// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/Gyroscope.h"

namespace leka::mock {

class Gyroscope : public interface::Gyroscope
{
  public:
	MOCK_METHOD((std::tuple<float, float, float>), getXYZ, (), (override));
};

}	// namespace leka::mock
