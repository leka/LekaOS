// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/FirmwareUpdate.h"
#include "mocks/leka/Timeout.h"

using namespace leka;
using namespace leka::system;
namespace bsml	= boost::sml;
namespace lksm	= system::robot::sm;
namespace event = system::robot::sm::event;

using testing::_;
using testing::AnyNumber;
using ::testing::MockFunction;
using testing::Return;

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

		ble::GapMock &mbed_mock_gap			= ble::gap_mock();
		ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

		EXPECT_CALL(mbed_mock_gatt, addService).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(AnyNumber());

		rc.initializeComponents();

		EXPECT_CALL(battery, level).Times(1);
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);
		EXPECT_CALL(sleep_timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));
		EXPECT_CALL(battery, onChargeDidStart).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_start));
		EXPECT_CALL(battery, onChargeDidStop).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_stop));
		EXPECT_CALL(sleep_timeout, start).Times(1);	  // Hide Uninteresting mock function call

		rc.registerEvents();
	}
	void TearDown() override { ble::delete_mocks(); }

	mock::Timeout sleep_timeout {};
	mock::Battery battery {};
	mock::FirmwareUpdate firmware_update {};

	VideoKit videokit {};

	RobotController<bsml::sm<robot::StateMachine, bsml::testing>> rc {sleep_timeout, battery, firmware_update,
																	  videokit};

	interface::Timeout::callback_t on_sleep_timeout = {};

	mbed::Callback<void()> on_charge_did_start {};
	mbed::Callback<void()> on_charge_did_stop {};
};

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}

TEST_F(RobotControllerTest, initializeComponents)
{
	ble::GapMock &mbed_mock_gap			= ble::gap_mock();
	ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

	EXPECT_CALL(mbed_mock_gatt, addService).Times(AnyNumber());
	EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
	EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

	rc.initializeComponents();
}

TEST_F(RobotControllerTest, stateSetupEventSetupComplete)
{
	ble::GapMock &mbed_mock_gap			= ble::gap_mock();
	ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

	rc.state_machine.set_current_states(lksm::state::setup);

	auto expected_level = 0x2A;
	EXPECT_CALL(battery, level).WillOnce(Return(expected_level));
	EXPECT_CALL(mbed_mock_gatt, write(_, sameValue(expected_level), _, _)).Times(1);

	EXPECT_CALL(sleep_timeout, onTimeout).Times(1);
	EXPECT_CALL(battery, onChargeDidStart).Times(1);
	EXPECT_CALL(battery, onChargeDidStop).Times(1);
	EXPECT_CALL(sleep_timeout, start).Times(1);

	rc.registerEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateIdleEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(sleep_timeout, stop).Times(1);

	on_sleep_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(sleep_timeout, stop).Times(1);

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));
	EXPECT_CALL(sleep_timeout, start).Times(1);

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
