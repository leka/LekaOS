// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGyroscope.h"

#include "gtest/gtest.h"
#include "mocks/leka/LSM6DSOX.h"

using namespace leka;

using testing::ReturnRef;

class CoreGyroscopeTest : public ::testing::Test
{
  protected:
	CoreGyroscopeTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::LSM6DSOX mock_lsm6dsox;
	CoreGyroscope gyro {mock_lsm6dsox};
};

TEST_F(CoreGyroscopeTest, initialization)
{
	ASSERT_NE(&gyro, nullptr);
}

TEST_F(CoreGyroscopeTest, getXYZ)
{
	leka::interface::LSM6DSOX::SensorData data {
		{12.F, 24.F, 36.F}, { 48.F, 60.F, 72.F }
	};

	EXPECT_CALL(mock_lsm6dsox, getData).WillOnce(ReturnRef(data));

	auto [gx, gy, gz] = gyro.getXYZ();

	EXPECT_EQ(data.gy.x, gx);
	EXPECT_EQ(data.gy.y, gy);
	EXPECT_EQ(data.gy.z, gz);
}
