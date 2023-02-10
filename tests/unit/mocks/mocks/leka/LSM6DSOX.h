// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/LSM6DSOX.hpp"

namespace leka::mock {

class LSM6DSOX : public interface::LSM6DSOX
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, registerOnGyDataReadyCallback, (std::function<void(const SensorData &)> const &), (override));
	MOCK_METHOD(void, registerOnDoubleTapCallback, (std::function<void()> const &), (override));
	MOCK_METHOD(void, setPowerMode, (PowerMode), (override));
};

}	// namespace leka::mock
