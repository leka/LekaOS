// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAccelerometer.h"

#include "gtest/gtest.h"
#include "mocks/leka/LSM6DSOX.h"

using namespace leka;

using ::testing::ReturnRef;

class CoreAccelerometerTest : public ::testing::Test
{
  protected:
	CoreAccelerometerTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::LSM6DSOX mock_lsm6dsox;
	CoreAccelerometer accel {mock_lsm6dsox};
};

TEST_F(CoreAccelerometerTest, initialization)
{
	ASSERT_NE(&accel, nullptr);
}

TEST_F(CoreAccelerometerTest, getXYZ)
{
	leka::interface::LSM6DSOX::SensorData data {
		{12.f, 24.f, 36.f}, { 48.f, 60.f, 72.f }
	};

	EXPECT_CALL(mock_lsm6dsox, getData).WillOnce(ReturnRef(data));

	auto [ax, ay, az] = accel.getXYZ();

	EXPECT_EQ(data.xl.x, ax);
	EXPECT_EQ(data.xl.y, ay);
	EXPECT_EQ(data.xl.z, az);
}
