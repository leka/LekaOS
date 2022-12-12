// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/LSM6DSOX.h"

namespace leka::mock {

class LSM6DSOX : public interface::LSM6DSOX
{
  public:
	LSM6DSOX();
	virtual ~LSM6DSOX();

	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, setPowerMode, (PowerMode), (override));
	MOCK_METHOD(SensorData &, getData, (), (override));
};

}	// namespace leka::mock
