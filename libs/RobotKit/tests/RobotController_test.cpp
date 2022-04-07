// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "ble_mocks.h"

#include "BehaviorKit.h"
#include "CommandKit.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "SerialNumberKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/EventQueue.h"
#include "mocks/leka/FirmwareUpdate.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/MCU.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/leka/Timeout.h"
#include "mocks/mbed/DigitalOut.h"
#include "mocks/mbed/EventFlags.h"

using namespace leka;
using namespace leka::system;
namespace bsml	= boost::sml;
namespace lksm	= system::robot::sm;
namespace event = system::robot::sm::event;

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;
using ::testing::Sequence;

ACTION_TEMPLATE(GetCallback, HAS_1_TEMPLATE_PARAMS(typename, callback_t), AND_1_VALUE_PARAMS(pointer))
{
	*pointer = callback_t(arg0);
}

MATCHER_P(sameValue, expected_value, "")
{
	auto same_value = arg[0] == expected_value;

	return same_value;
}

class RobotControllerTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		ble::init_mocks();

		expectedCallsInitializeComponents();
		expectedCallsRegisterEvents();
	}
	void TearDown() override { ble::delete_mocks(); }

	mock::EventQueue event_queue {};

	mock::Timeout sleep_timeout {};
	mock::Battery battery {};

	mock::MCU mock_mcu {};
	SerialNumberKit serialnumberkit {mock_mcu};

	mock::FirmwareUpdate firmware_update {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};
	rtos::Thread animation_thread;
	mbed::mock::EventFlags mock_event_flags;

	LedKit ledkit {animation_thread, mock_event_flags, ears, belt};

	mock::LEDAnimation mock_animation {};

	mbed::mock::DigitalOut dir_1_left = {};
	mbed::mock::DigitalOut dir_2_left = {};
	mock::PwmOut speed_left			  = {};

	mbed::mock::DigitalOut dir_1_right = {};
	mbed::mock::DigitalOut dir_2_right = {};
	mock::PwmOut speed_right		   = {};

	CoreMotor motor_left {dir_1_left, dir_2_left, speed_left};
	CoreMotor motor_right {dir_1_right, dir_2_right, speed_right};

	VideoKit videokit {};

	BehaviorKit bhvkit {videokit, ledkit, motor_left, motor_right};

	CommandKit cmdkit {};

	RobotController<bsml::sm<robot::StateMachine, bsml::testing>> rc {
		sleep_timeout, battery, serialnumberkit, firmware_update, motor_left,
		motor_right,   ledkit,	videokit,		 bhvkit,		  cmdkit};

	ble::GapMock &mbed_mock_gap			= ble::gap_mock();
	ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

	interface::Timeout::callback_t on_sleep_timeout = {};

	mbed::Callback<void()> on_charge_did_start {};
	mbed::Callback<void()> on_charge_did_stop {};

	bool spy_isCharging_return_value = false;

	void expectedCallsStopMotors()
	{
		EXPECT_CALL(dir_1_left, write(0));
		EXPECT_CALL(dir_2_left, write(0));
		EXPECT_CALL(speed_left, write(0));
		EXPECT_CALL(dir_1_right, write(0));
		EXPECT_CALL(dir_2_right, write(0));
		EXPECT_CALL(speed_right, write(0));
	}

	void expectedCallsInitializeComponents()
	{
		{
			InSequence seq;

			EXPECT_CALL(mbed_mock_gatt, addService).Times(4);
			EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
			EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

			EXPECT_CALL(mock_mcu, getID).Times(1);
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);

			expectedCallsStopMotors();
		}

		rc.initializeComponents();
	}

	void expectedCallsRegisterEvents()
	{
		{
			InSequence seq;

			Sequence on_low_battery_sequence;
			EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);
			EXPECT_CALL(battery, isCharging)
				.InSequence(on_low_battery_sequence)
				.WillOnce(Return(spy_isCharging_return_value));
			EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

			Sequence on_data_updated_sequence;
			EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

			EXPECT_CALL(sleep_timeout, onTimeout)
				.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));

			EXPECT_CALL(battery, onChargeDidStart).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_start));

			EXPECT_CALL(battery, onChargeDidStop).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_stop));

			{
				InSequence event_setup_complete;

				if (spy_isCharging_return_value == true) {
					expectedCallsTransitionSetupToCharging();
				} else {
					expectedCallsTransitionSetupToIdle();
				}
			}
		}

		rc.registerEvents();
	}

	void expectedCallsTransitionSetupToIdle()
	{
		Sequence is_charging_sequence;

		EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(spy_isCharging_return_value));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

		EXPECT_CALL(sleep_timeout, start);
	}

	void expectedCallsTransitionSetupToCharging()
	{
		Sequence is_charging_sequence;

		EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(spy_isCharging_return_value));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

		EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(spy_isCharging_return_value));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);
	}
};

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}

TEST_F(RobotControllerTest, initializeComponents)
{
	{
		InSequence seq;

		EXPECT_CALL(mbed_mock_gatt, addService).Times(4);
		EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
		EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

		EXPECT_CALL(mock_mcu, getID).Times(1);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);

		expectedCallsStopMotors();
	}

	rc.initializeComponents();
}

TEST_F(RobotControllerTest, registerEventsBatteryIsNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	{
		InSequence seq;

		Sequence on_low_battery_sequence;
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);
		EXPECT_CALL(battery, isCharging).InSequence(on_low_battery_sequence).WillOnce(Return(false));
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

		EXPECT_CALL(sleep_timeout, onTimeout);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(false));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			EXPECT_CALL(sleep_timeout, start);
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, registerEventsBatteryIsCharging)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	{
		InSequence seq;

		Sequence on_low_battery_sequence;
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);
		EXPECT_CALL(battery, isCharging).InSequence(on_low_battery_sequence).WillOnce(Return(true));
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

		EXPECT_CALL(sleep_timeout, onTimeout);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(true));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(true));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelBelow25)
{
	auto battery_level = 0;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove5Below25)
{
	auto battery_level = 22;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove25Below50)
{
	auto battery_level = 42;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove50Below75)
{
	auto battery_level = 66;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove75)
{
	auto battery_level = 90;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, stateSetupEventSetupCompleteGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	spy_isCharging_return_value = false;
	expectedCallsRegisterEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateSetupEventSetupCompleteGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	spy_isCharging_return_value = true;
	expectedCallsRegisterEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateIdleEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(sleep_timeout, stop).Times(1);

	expectedCallsStopMotors();

	on_sleep_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateSleepingEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	// TODO: Trigger command_received in StateMachine from BLE

	// EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));
	expectedCallsStopMotors();

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(sleep_timeout, stop).Times(1);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));
	expectedCallsStopMotors();

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

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
