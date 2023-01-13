// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "CoreLSM6DSOX.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using testing::_;
using testing::AtLeast;

class CoreLSM6DSOXTest : public ::testing::Test
{
  protected:
	CoreLSM6DSOXTest() = default;
	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreI2C mocki2c {};
	CoreLSM6DSOX lsm6dsox {mocki2c};
};

TEST_F(CoreLSM6DSOXTest, initialization)
{
	ASSERT_NE(&lsm6dsox, nullptr);
}

TEST_F(CoreLSM6DSOXTest, i2cCommunicationWorking)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	lsm6dsox.init();
}

TEST_F(CoreLSM6DSOXTest, setPowerMode)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::UltraLow);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::High);
}

TEST_F(CoreLSM6DSOXTest, getData)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	lsm6dsox.getData();
}
