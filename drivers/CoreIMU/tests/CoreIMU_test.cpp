// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "CoreIMU.hpp"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/EventQueue.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;

using testing::_;
using testing::AnyNumber;
using testing::AtLeast;
using testing::MockFunction;

class CoreIMUTest : public ::testing::Test
{
  protected:
	CoreIMUTest() = default;

	void SetUp() override
	{
		EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
		EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

		coreimu.init();
	}
	// void TearDown() override {}

	mock::CoreI2C mocki2c {};
	CoreInterruptIn irq {NC};

	CoreIMU coreimu {mocki2c, irq};

	// ? Instantiation of mock::EventQueue is needed to setup the underlying stubs that will make the mock work
	// ? correctly. Without it UT are failing
	// TODO (@ladislas) - review mocks/stubs to remove the need of the object, replace with setup/teardown functions
	mock::EventQueue _ {};
};

TEST_F(CoreIMUTest, initialization)
{
	ASSERT_NE(&coreimu, nullptr);
}

TEST_F(CoreIMUTest, setPowerMode)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::Off);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::UltraLow);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::High);
}

TEST_F(CoreIMUTest, onDataReady)
{
	MockFunction<void(const leka::interface::IMU::SensorData &data)> mock_callback;

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	EXPECT_CALL(mock_callback, Call).Times(1);

	coreimu.registerOnDataReadyCallback(mock_callback.AsStdFunction());

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreIMUTest, emptyOnDataReadyCallback)
{
	coreimu.registerOnDataReadyCallback({});

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreIMUTest, enableOnDataReadyInterrupt)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	coreimu.enableOnDataReadyInterrupt();
}

TEST_F(CoreIMUTest, disableOnDataReadyInterrupt)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	coreimu.disableOnDataReadyInterrupt();
}

TEST_F(CoreIMUTest, enableDeepSleep)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::Off);

	coreimu.enableDeepSleep();
}

TEST_F(CoreIMUTest, disableDeepSleep)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

	coreimu.disableDeepSleep();
}

TEST_F(CoreIMUTest, onWakeUpCallback)
{
	MockFunction<void()> mock_callback;

	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));
	EXPECT_CALL(mock_callback, Call).Times(AnyNumber());

	coreimu.registerOnWakeUpCallback(mock_callback.AsStdFunction());

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreIMUTest, emptyOnWakeUpCallback)
{
	coreimu.registerOnWakeUpCallback({});

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreIMUTest, enableOnWakeUpInterrupt)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	coreimu.enableOnWakeUpInterrupt();
}

TEST_F(CoreIMUTest, disableOnWakeUpInterrupt)
{
	EXPECT_CALL(mocki2c, write).Times(AtLeast(1));
	EXPECT_CALL(mocki2c, read).Times(AtLeast(1));

	coreimu.disableOnWakeUpInterrupt();
}

TEST_F(CoreIMUTest, switchCallbacks)
{
	auto mock_data_available_callback = MockFunction<void(const leka::interface::IMU::SensorData &data)> {};
	auto mock_wake_up_callback		  = MockFunction<void()> {};
	auto on_rise_callback			  = mbed::Callback<void()> {};

	EXPECT_CALL(mocki2c, write).Times(AnyNumber());
	EXPECT_CALL(mocki2c, read).Times(AnyNumber());

	coreimu.registerOnDataReadyCallback(mock_data_available_callback.AsStdFunction());
	coreimu.registerOnWakeUpCallback(mock_wake_up_callback.AsStdFunction());

	{
		// Enable Data Available
		EXPECT_CALL(mock_data_available_callback, Call).Times(1);
		EXPECT_CALL(mock_wake_up_callback, Call).Times(0);
		coreimu.enableOnDataReadyInterrupt();
		on_rise_callback = spy_InterruptIn_getRiseCallback();
		on_rise_callback();
	}

	{
		// Disable Data Available
		EXPECT_CALL(mock_data_available_callback, Call).Times(0);
		EXPECT_CALL(mock_wake_up_callback, Call).Times(0);
		coreimu.disableOnDataReadyInterrupt();
		on_rise_callback = spy_InterruptIn_getRiseCallback();
		on_rise_callback();
	}

	{
		// Enable Wake Up
		EXPECT_CALL(mock_data_available_callback, Call).Times(0);
		EXPECT_CALL(mock_wake_up_callback, Call).Times(AnyNumber());
		coreimu.enableOnWakeUpInterrupt();
		on_rise_callback = spy_InterruptIn_getRiseCallback();
		on_rise_callback();
	}

	{
		// Disable Wake Up
		EXPECT_CALL(mock_data_available_callback, Call).Times(0);
		EXPECT_CALL(mock_wake_up_callback, Call).Times(0);
		coreimu.disableOnWakeUpInterrupt();
		on_rise_callback = spy_InterruptIn_getRiseCallback();
		on_rise_callback();
	}
}
