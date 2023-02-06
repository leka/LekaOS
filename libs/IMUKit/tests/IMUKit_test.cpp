// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.hpp"

#include "CoreLSM6DSOX.hpp"
#include "ThisThread.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/LSM6DSOX.h"
#include "stubs/leka/EventLoopKit.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::SaveArg;

class IMUKitTest : public ::testing::Test
{
  protected:
	IMUKitTest() = default;

	void SetUp() override { imukit.init(); }
	// void TearDown() override {}

	mock::LSM6DSOX mock_lsm6dox {};

	IMUKit imukit {mock_lsm6dox};
};

TEST_F(IMUKitTest, initialization)
{
	ASSERT_NE(&imukit, nullptr);
}

TEST_F(IMUKitTest, start)
{
	EXPECT_CALL(mock_lsm6dox, setPowerMode(interface::LSM6DSOX::PowerMode::Normal)).Times(1);

	imukit.start();
}

TEST_F(IMUKitTest, stop)
{
	EXPECT_CALL(mock_lsm6dox, setPowerMode(interface::LSM6DSOX::PowerMode::Off)).Times(1);

	imukit.stop();
}
