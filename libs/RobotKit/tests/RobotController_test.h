// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? Include order is important here
// clang-format off
#include "RobotController.h"
// clang-format on

#include "ble_mocks.h"

#include "BehaviorKit.h"
#include "CommandKit.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "SerialNumberKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/CoreLCD.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/EventQueue.h"
#include "mocks/leka/FirmwareUpdate.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/MCU.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/leka/Timeout.h"
#include "mocks/leka/VideoKit.h"
#include "mocks/mbed/DigitalOut.h"
#include "mocks/mbed/EventFlags.h"

using namespace leka;
namespace bsml = boost::sml;
namespace lksm = system::robot::sm;

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

	mock::Timeout timeout {};
	mock::Battery battery {};

	mock::MCU mock_mcu {};
	SerialNumberKit serialnumberkit {mock_mcu};

	mock::FirmwareUpdate firmware_update {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	rtos::Thread animation_thread;
	mbed::mock::EventFlags mock_event_flags;

	LedKit ledkit {animation_thread, mock_event_flags, mock_ears, mock_belt};

	mock::LEDAnimation mock_animation {};

	mbed::mock::DigitalOut dir_1_left = {};
	mbed::mock::DigitalOut dir_2_left = {};
	mock::PwmOut speed_left			  = {};

	mbed::mock::DigitalOut dir_1_right = {};
	mbed::mock::DigitalOut dir_2_right = {};
	mock::PwmOut speed_right		   = {};

	CoreMotor motor_left {dir_1_left, dir_2_left, speed_left};
	CoreMotor motor_right {dir_1_right, dir_2_right, speed_right};

	mock::CoreLCD mock_lcd {};
	mock::VideoKit mock_videokit {};

	BehaviorKit bhvkit {mock_videokit, ledkit, motor_left, motor_right};

	CommandKit cmdkit {};

	RobotController<bsml::sm<system::robot::StateMachine, bsml::testing>> rc {
		timeout,   battery, serialnumberkit, firmware_update, motor_left, motor_right, mock_ears,
		mock_belt, ledkit,	mock_lcd,		 mock_videokit,	  bhvkit,	  cmdkit};

	ble::GapMock &mbed_mock_gap			= ble::gap_mock();
	ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

	interface::Timeout::callback_t on_sleep_timeout			 = {};
	interface::Timeout::callback_t on_sleeping_start_timeout = {};
	interface::Timeout::callback_t on_charging_start_timeout = {};

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

			EXPECT_CALL(mbed_mock_gatt, addService).Times(6);
			EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
			EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

			EXPECT_CALL(mock_mcu, getID).Times(1);
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);

			expectedCallsStopMotors();

			EXPECT_CALL(mock_videokit, initializeScreen).Times(1);
			EXPECT_CALL(mock_lcd, turnOff).Times(1);
			EXPECT_CALL(mock_videokit, stopVideo).Times(1);
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
			EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/loading.jpg"}))
				.Times(AnyNumber());
			EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

			Sequence on_data_updated_sequence;
			EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

			EXPECT_CALL(timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));

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

		expectedCallsRunLaunchingBehavior();

		Sequence on_idle_entry_sequence;
		EXPECT_CALL(timeout, onTimeout)
			.InSequence(on_idle_entry_sequence)
			.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));
		EXPECT_CALL(timeout, start).InSequence(on_idle_entry_sequence);

		EXPECT_CALL(mock_videokit, playVideo).InSequence(on_idle_entry_sequence);
		EXPECT_CALL(mock_lcd, turnOn).InSequence(on_idle_entry_sequence);
	}

	void expectedCallsTransitionSetupToCharging()
	{
		Sequence is_charging_sequence;

		EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(spy_isCharging_return_value));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

		EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(spy_isCharging_return_value));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

		expectedCallsRunLaunchingBehavior();

		Sequence start_charging_behavior_sequence;
		EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
		EXPECT_CALL(timeout, onTimeout)
			.InSequence(start_charging_behavior_sequence)
			.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_charging_start_timeout));
		EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);
	}

	void expectedCallsRunLaunchingBehavior()
	{
		InSequence run_launching_behavior_sequence;

		EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/logo.jpg"})).Times(1);
		EXPECT_CALL(mock_lcd, turnOn);
	}
};
