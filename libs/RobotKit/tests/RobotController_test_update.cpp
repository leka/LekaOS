// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, isReadyToUpdateVersionIsNotAvailable)
{
	auto actual_is_ready = bool {};
	auto minimal_battery = uint8_t {25};

	EXPECT_CALL(firmware_update, isVersionAvailable).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);
}

TEST_F(RobotControllerTest, isReadyToUpdateVersionIsAvailable)
{
	auto actual_is_ready = bool {};
	auto minimal_battery = uint8_t {25};

	EXPECT_CALL(firmware_update, isVersionAvailable).WillRepeatedly(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToUpdate();
	EXPECT_TRUE(actual_is_ready);
}

TEST_F(RobotControllerTest, applyUpdateCallbackNotRegistered)
{
	EXPECT_CALL(firmware_update, loadFirmware).WillOnce(Return(false));
	rc.applyUpdate();

	EXPECT_CALL(firmware_update, loadFirmware).WillOnce(Return(true));
	rc.applyUpdate();
}

TEST_F(RobotControllerTest, applyUpdateCallbackRegistered)
{
	MockFunction<void()> callback;
	rc.registerOnUpdateLoadedCallback(callback.AsStdFunction());

	EXPECT_CALL(firmware_update, loadFirmware).WillOnce(Return(false));
	EXPECT_CALL(callback, Call).Times(0);
	rc.applyUpdate();

	EXPECT_CALL(firmware_update, loadFirmware).WillOnce(Return(true));
	EXPECT_CALL(callback, Call).Times(1);
	rc.applyUpdate();
}

TEST_F(RobotControllerTest, registerOnFactoryResetNotificationCallback)
{
	MockFunction<void(bool)> callback {};

	rc.registerOnFactoryResetNotificationCallback(callback.AsStdFunction());

	// nothing expected
}
