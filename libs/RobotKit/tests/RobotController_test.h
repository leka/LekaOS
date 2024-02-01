// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? Include order is important here
// clang-format off
#include "RobotController.h"
// clang-format on

#include "ble_mocks.h"

#include "ActivityKit.h"
#include "BehaviorKit.h"
#include "CommandKit.h"
#include "CoreBufferedSerial.h"
#include "CorePwm.h"
#include "CoreRFIDReaderCR95HF.h"
#include "DisplayTags.h"
#include "LedKitAnimations.h"
#include "RFIDKit.h"
#include "SerialNumberKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/CoreLCD.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/EventQueue.h"
#include "mocks/leka/FirmwareUpdate.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/LedKit.h"
#include "mocks/leka/MCU.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/leka/Timeout.h"
#include "mocks/leka/VideoKit.h"
#include "mocks/mbed/DigitalOut.h"
#include "stubs/leka/EventLoopKit.h"
#include "stubs/mbed/Kernel.h"

using namespace leka;
using namespace std::chrono_literals;
namespace bsml = boost::sml;
namespace lksm = system::robot::sm;

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Sequence;

ACTION_TEMPLATE(GetCallback, HAS_1_TEMPLATE_PARAMS(typename, callback_t), AND_1_VALUE_PARAMS(pointer))
{
	*pointer = callback_t(arg0);
}

MATCHER_P(isSameAnimation, expected_animation_type, "")
{
	bool is_same = typeid(*expected_animation_type) == typeid(*arg);
	return is_same;
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

	mock::Timeout timeout_state_internal {};
	mock::Timeout timeout_state_transition {};
	mock::Timeout timeout_autonomous_activities {};
	mock::Battery battery {};

	mock::MCU mock_mcu {};
	SerialNumberKit serialnumberkit {mock_mcu, SerialNumberKit::DEFAULT_CONFIG};

	mock::FirmwareUpdate firmware_update {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	mock::LedKit mock_ledkit;

	mock::LEDAnimation mock_animation {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::CoreLCD mock_lcd {};
	mock::VideoKit mock_videokit {};

	BehaviorKit bhvkit {mock_videokit, mock_ledkit, mock_motor_left, mock_motor_right};

	CoreBufferedSerial serial {RFID_UART_TX, RFID_UART_RX, 57600};
	CoreRFIDReaderCR95HF reader {serial};
	RFIDKit rfidkit {reader};

	ActivityKit activitykit {mock_videokit};
	activity::DisplayTags display_tag {rfidkit, mock_videokit};

	stub::EventLoopKit event_loop {};
	CommandKit cmdkit {event_loop};

	RobotController<bsml::sm<system::robot::StateMachine, bsml::testing>> rc {timeout_state_internal,
																			  timeout_state_transition,
																			  timeout_autonomous_activities,
																			  battery,
																			  serialnumberkit,
																			  firmware_update,
																			  mock_motor_left,
																			  mock_motor_right,
																			  mock_ears,
																			  mock_belt,
																			  mock_ledkit,
																			  mock_lcd,
																			  mock_videokit,
																			  bhvkit,
																			  cmdkit,
																			  rfidkit,
																			  activitykit};

	ble::GapMock &mbed_mock_gap			= ble::gap_mock();
	ble::GattServerMock &mbed_mock_gatt = ble::gatt_server_mock();

	interface::Timeout::callback_t on_sleep_timeout					= {};
	interface::Timeout::callback_t on_deep_sleep_timeout			= {};
	interface::Timeout::callback_t on_idle_timeout					= {};
	interface::Timeout::callback_t on_sleeping_start_timeout		= {};
	interface::Timeout::callback_t on_charging_start_timeout		= {};
	interface::Timeout::callback_t on_autonomous_activities_timeout = {};

	std::function<void()> on_charge_did_start {};
	std::function<void()> on_charge_did_stop {};

	bool spy_isCharging_return_value = false;

	void expectedCallsStopActuators()
	{
		EXPECT_CALL(mock_ledkit, stop).Times(AtLeast(1));
		EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
		EXPECT_CALL(mock_motor_left, stop()).Times(AtLeast(1));
		EXPECT_CALL(mock_motor_right, stop()).Times(AtLeast(1));
		EXPECT_CALL(mock_belt, hide).Times(AtLeast(1));
		EXPECT_CALL(mock_ears, hide).Times(AtLeast(1));
	}

	void expectedCallsInitializeComponents()
	{
		EXPECT_CALL(mock_mcu, getID).Times(AnyNumber());
		EXPECT_CALL(firmware_update, getCurrentVersion).Times(AnyNumber());

		EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gatt, addService).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

		EXPECT_CALL(mock_ledkit, init).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, initializeScreen).Times(AnyNumber());
		EXPECT_CALL(mock_lcd, turnOff).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		rc.initializeComponents();
	}

	void expectedCallsRegisterEvents()
	{
		EXPECT_CALL(battery, isCharging).Times(AnyNumber());
		EXPECT_CALL(battery, level).Times(AnyNumber());

		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, displayImage).Times(AnyNumber());

		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(AnyNumber());
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

		{
			EXPECT_CALL(timeout_state_transition, onTimeout).Times(AnyNumber());
			EXPECT_CALL(timeout_state_transition, start).Times(AnyNumber());
			EXPECT_CALL(mock_videokit, playVideoOnRepeat).Times(AnyNumber());
			EXPECT_CALL(mock_lcd, turnOn).Times(AnyNumber());
		}	// ? On Idle entry

		// Saved callback
		EXPECT_CALL(battery, onChargeDidStart).WillOnce(SaveArg<0>(&on_charge_did_start));
		EXPECT_CALL(battery, onChargeDidStop).WillOnce(SaveArg<0>(&on_charge_did_stop));

		rc.registerEvents();
	}

	void expectedCallsResetAutonomousActivitiesTimeout()
	{
		auto expected_duration = std::chrono::seconds {600};
		{
			InSequence seq;

			EXPECT_CALL(timeout_autonomous_activities, stop);
			EXPECT_CALL(timeout_autonomous_activities, onTimeout)
				.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_autonomous_activities_timeout));
			EXPECT_CALL(timeout_autonomous_activities, start(std::chrono::microseconds {expected_duration}));
		}
	}
};
