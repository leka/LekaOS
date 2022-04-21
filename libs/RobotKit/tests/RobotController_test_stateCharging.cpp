// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));
	EXPECT_CALL(sleep_timeout, start).Times(1);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateChargingEventUpdateRequestedGuardIsReadyToUpdateFalseNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	uint8_t returned_level {100};

	// TODO (@yann): Trigger update_requested in StateMachine from BLE and remove isReadyToUpdate call
	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));
	EXPECT_CALL(firmware_update, loadUpdate).Times(0);
	rc.isReadyToUpdate();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingEventUpdateRequestedGuardIsReadyToUpdateFalseBelowMinimalBatteryLevel)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	uint8_t returned_level {0};

	// TODO (@yann): Trigger update_requested in StateMachine from BLE and remove isReadyToUpdate call
	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(battery, level).WillOnce(Return(returned_level));
	EXPECT_CALL(firmware_update, loadUpdate).Times(0);
	rc.isReadyToUpdate();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingEventUpdateRequestedGuardIsReadyToUpdateTrueLoadUpdateSuccess)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	uint8_t returned_level {100};

	MockFunction<void()> mock_on_update_loaded_callback;
	rc.registerOnUpdateLoadedCallback(mock_on_update_loaded_callback.AsStdFunction());

	// TODO (@yann): Trigger update_requested in StateMachine from BLE and remove isReadyToUpdate and applyUpdate calls

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(battery, level).WillOnce(Return(returned_level));
	EXPECT_CALL(firmware_update, loadUpdate).WillOnce(Return(true));
	EXPECT_CALL(mock_on_update_loaded_callback, Call).Times(1);
	rc.isReadyToUpdate();
	rc.applyUpdate();

	// EXPECT_TRUE(rc.state_machine.is(lksm::state::updating));
}

TEST_F(RobotControllerTest,
	   stateChargingEventUpdateRequestedGuardIsReadyToUpdateTrueLoadUpdateSuccessOnUpdateLoadedCallbackFail)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	uint8_t returned_level {100};

	rc.registerOnUpdateLoadedCallback(nullptr);

	// TODO (@yann): Trigger update_requested in StateMachine from BLE and remove isReadyToUpdate and applyUpdate calls

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(battery, level).WillOnce(Return(returned_level));
	EXPECT_CALL(firmware_update, loadUpdate).WillOnce(Return(true));
	rc.isReadyToUpdate();
	rc.applyUpdate();

	// EXPECT_TRUE(rc.state_machine.is(lksm::state::updating));
}

TEST_F(RobotControllerTest, stateChargingEventUpdateRequestedGuardIsReadyToUpdateTrueLoadUpdateFail)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	uint8_t returned_level {100};

	MockFunction<void()> mock_on_update_loaded_callback;
	rc.registerOnUpdateLoadedCallback(mock_on_update_loaded_callback.AsStdFunction());

	// TODO (@yann): Trigger update_requested in StateMachine from BLE and remove isReadyToUpdate and applyUpdate calls

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(battery, level).WillOnce(Return(returned_level));
	EXPECT_CALL(firmware_update, loadUpdate).WillOnce(Return(false));
	EXPECT_CALL(mock_on_update_loaded_callback, Call).Times(0);
	rc.isReadyToUpdate();
	rc.applyUpdate();

	// EXPECT_TRUE(rc.state_machine.is(lksm::state::updating));
}
