// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BatteryKit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/EventQueue.h"

using namespace leka;

using ::testing::MockFunction;
using ::testing::Return;
using ::testing::SaveArg;

class BatteryKitTest : public ::testing::Test
{
  protected:
	BatteryKitTest() : batterykit(mock_battery) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::EventQueue event_queue {};

	mock::Battery mock_battery;
	BatteryKit batterykit;
};

TEST_F(BatteryKitTest, instantiation)
{
	EXPECT_NE(&batterykit, nullptr);
}

TEST_F(BatteryKitTest, level)
{
	auto expected_value = 0x2A;

	EXPECT_CALL(mock_battery, level).WillOnce(Return(expected_value));

	auto actual_value = batterykit.level();

	EXPECT_EQ(actual_value, expected_value);
}

TEST_F(BatteryKitTest, isCharging)
{
	auto expected_value = true;

	EXPECT_CALL(mock_battery, isCharging).WillOnce(Return(expected_value));

	auto actual_value = batterykit.isCharging();

	EXPECT_EQ(actual_value, expected_value);
}

TEST_F(BatteryKitTest, onChargeDidStart)
{
	MockFunction<void()> callback {};
	std::function<void()> charge_did_start_callback {};

	EXPECT_CALL(mock_battery, onChargeDidStart).WillOnce(SaveArg<0>(&charge_did_start_callback));
	batterykit.onChargeDidStart(callback.AsStdFunction());

	EXPECT_CALL(callback, Call);
	charge_did_start_callback();
}

TEST_F(BatteryKitTest, onChargeDidStop)
{
	MockFunction<void()> callback;
	std::function<void()> charge_did_stop_callback;

	EXPECT_CALL(mock_battery, onChargeDidStop).WillOnce(SaveArg<0>(&charge_did_stop_callback));
	batterykit.onChargeDidStop(callback.AsStdFunction());

	EXPECT_CALL(callback, Call);
	charge_did_stop_callback();
}

TEST_F(BatteryKitTest, onDataUpdated)
{
	auto battery_level = 0x2A;
	MockFunction<void(uint8_t)> mock_on_data_updated_callback;

	batterykit.onDataUpdated(mock_on_data_updated_callback.AsStdFunction());

	EXPECT_CALL(mock_battery, level).WillRepeatedly(Return(battery_level));
	EXPECT_CALL(mock_on_data_updated_callback, Call(battery_level)).Times(1);

	batterykit.startEventHandler();
}

TEST_F(BatteryKitTest, onLowBattery)
{
	auto battery_level = 0x2A;
	MockFunction<void()> mock_on_low_battery_callback;

	batterykit.onLowBattery(mock_on_low_battery_callback.AsStdFunction());

	EXPECT_CALL(mock_battery, level).WillRepeatedly(Return(battery_level));
	EXPECT_CALL(mock_on_low_battery_callback, Call).Times(0);

	batterykit.startEventHandler();
}

TEST_F(BatteryKitTest, onLowBatteryLevelAtZero)
{
	auto battery_level = 0x00;
	MockFunction<void()> mock_on_low_battery_callback;

	batterykit.onLowBattery(mock_on_low_battery_callback.AsStdFunction());

	EXPECT_CALL(mock_battery, level).WillRepeatedly(Return(battery_level));
	EXPECT_CALL(mock_on_low_battery_callback, Call).Times(1);

	batterykit.startEventHandler();
}
