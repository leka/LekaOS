// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "CoreLSM6DSOX.hpp"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/EventQueue.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;

using testing::_;
using testing::AtLeast;
using testing::MockFunction;

class CoreLSM6DSOXTest : public ::testing::Test
{
  protected:
	CoreLSM6DSOXTest() = default;

	void SetUp() override
	{
		EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
		EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

		lsm6dsox.init();
	}
	// void TearDown() override {}

	mock::CoreI2C mocki2c {};
	CoreInterruptIn drdy_irq {NC};

	CoreLSM6DSOX lsm6dsox {mocki2c, drdy_irq};

	// ? Instantiation of mock::EventQueue is needed to setup the underlying stubs that will make the mock work
	// ? correctly. Without it UT are failing
	// TODO (@ladislas) - review mocks/stubs to remove the need of the object, replace with setup/teardown functions
	mock::EventQueue _ {};
};

TEST_F(CoreLSM6DSOXTest, initialization)
{
	ASSERT_NE(&lsm6dsox, nullptr);
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

TEST_F(CoreLSM6DSOXTest, onGyrDRDY)
{
	MockFunction<void(const leka::interface::LSM6DSOX::SensorData &data)> mock_callback;

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	EXPECT_CALL(mock_callback, Call).Times(1);

	lsm6dsox.registerOnGyDataReadyCallback(mock_callback.AsStdFunction());

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreLSM6DSOXTest, onDoubleTap)
{
	lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::UltraLow);

	MockFunction<void()> mock_callback;

	EXPECT_CALL(mock_callback, Call).Times(1);

	lsm6dsox.registerOnDoubleTapCallback(mock_callback.AsStdFunction());

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}
