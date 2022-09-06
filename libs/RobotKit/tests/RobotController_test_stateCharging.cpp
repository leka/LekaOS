// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, onChargingStartTimeout)
{
	EXPECT_CALL(battery, level);
	EXPECT_CALL(mock_videokit, displayImage);
	EXPECT_CALL(mock_lcd, turnOn).Times(AnyNumber());
	EXPECT_CALL(timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_charging_start_timeout));
	EXPECT_CALL(timeout, start).Times(AnyNumber());
	rc.startChargingBehavior();

	EXPECT_CALL(mock_lcd, turnOff);
	on_charging_start_timeout();
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingConnectedEventChargeDidStopGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::charging, lksm::state::connected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(timeout, stop);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

TEST_F(RobotControllerTest, stateChargingDisconnectedEventChargeDidStopGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::charging, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(timeout, stop);

	Sequence on_idle_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnRepeat).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_idle_entry_sequence);

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

TEST_F(RobotControllerTest, stateChargingEventEmergencyStop)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	EXPECT_CALL(timeout, stop);

	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_lcd, turnOff).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo).Times(2);

	rc.onMagicCardAvailable(MagicCard::emergency_stop);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}

TEST_F(RobotControllerTest, stateChargingEventAutonomousActivityRequested)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, level);
	EXPECT_CALL(mock_videokit, displayImage).Times(1);
	EXPECT_CALL(mock_lcd, turnOn).Times(AnyNumber());
	EXPECT_CALL(timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_charging_start_timeout));
	EXPECT_CALL(timeout, start).Times(AnyNumber());

	rc.onMagicCardAvailable(MagicCard::remote_standard);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}
