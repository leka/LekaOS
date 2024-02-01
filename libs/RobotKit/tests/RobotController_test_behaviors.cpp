// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, runLaunchingBehavior)
{
	{
		EXPECT_CALL(mock_videokit,
					displayImage(std::filesystem::path {"/fs/home/img/system/robot-misc-splash_screen-large-400.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, launching);
	EXPECT_CALL(mock_lcd, turnOn);

	rc.runLaunchingBehavior();
}

TEST_F(RobotControllerTest, startWaitingBehavior)
{
	{
		EXPECT_CALL(
			mock_videokit,
			playVideoOnRepeat(
				std::filesystem::path {"/fs/home/vid/system/robot-system-idle-looking_top_right_left-no_eyebrows.avi"},
				_));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, waiting);
	EXPECT_CALL(mock_lcd, turnOn);

	rc.startWaitingBehavior();
}

TEST_F(RobotControllerTest, stopWaitingBehavior)
{
	{
		EXPECT_CALL(mock_ledkit, stop);
		EXPECT_CALL(mock_videokit, stopVideo);
		EXPECT_CALL(mock_motor_left, stop);
		EXPECT_CALL(mock_motor_right, stop);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, stop);

	rc.stopWaitingBehavior();
}

TEST_F(RobotControllerTest, startWorkingBehavior)
{
	{
		EXPECT_CALL(mock_videokit,
					displayImage(std::filesystem::path {"/fs/home/img/system/robot-face-smiling-slightly.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, working);
	EXPECT_CALL(mock_lcd, turnOn);

	rc.startWorkingBehavior();
}

TEST_F(RobotControllerTest, startSleepingBehavior)
{
	{
		EXPECT_CALL(
			mock_videokit,
			playVideoOnce(
				std::filesystem::path {"/fs/home/vid/system/robot-system-sleep-yawn_then_sleep-no_eyebrows.avi"}, _));
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::sleeping)));

	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, sleeping);
	EXPECT_CALL(mock_lcd, turnOn);

	Sequence seq;
	EXPECT_CALL(timeout_state_internal, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleeping_start_timeout));
	EXPECT_CALL(timeout_state_internal, start).InSequence(seq);

	rc.startSleepingBehavior();

	EXPECT_CALL(mock_lcd, turnOff);
	EXPECT_CALL(mock_ledkit, stop);

	on_sleeping_start_timeout();
}

TEST_F(RobotControllerTest, stopSleepingBehavior)
{
	{
		EXPECT_CALL(mock_ledkit, stop);
		EXPECT_CALL(mock_videokit, stopVideo);
		EXPECT_CALL(mock_motor_left, stop);
		EXPECT_CALL(mock_motor_right, stop);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, stop);

	EXPECT_CALL(timeout_state_internal, stop);

	rc.stopSleepingBehavior();
}

TEST_F(RobotControllerTest, onChargingBehavior)
{
	auto level = uint8_t {};

	level = 10;
	{
		EXPECT_CALL(mock_videokit,
					displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-empty_red.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingEmpty);
	rc.onChargingBehavior(level);

	level = 33;
	{
		EXPECT_CALL(
			mock_videokit,
			displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_1-red.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingLow);
	rc.onChargingBehavior(level);

	level = 66;
	{
		EXPECT_CALL(
			mock_videokit,
			displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_2-orange.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingMedium);
	rc.onChargingBehavior(level);

	level = 80;
	{
		EXPECT_CALL(
			mock_videokit,
			displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_3-green.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingHigh);
	rc.onChargingBehavior(level);

	level = 100;
	{
		EXPECT_CALL(
			mock_videokit,
			displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_4-green.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingFull);
	rc.onChargingBehavior(level);
}

TEST_F(RobotControllerTest, startChargingBehavior)
{
	EXPECT_CALL(battery, level).WillRepeatedly(Return(0));
	{
		EXPECT_CALL(mock_videokit, displayImage);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingEmpty);
	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge)));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, blinkOnCharge);
	EXPECT_CALL(mock_lcd, turnOn);

	Sequence seq;
	EXPECT_CALL(timeout_state_internal, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_charging_start_timeout));
	EXPECT_CALL(timeout_state_internal, start).InSequence(seq);

	rc.startChargingBehavior();

	EXPECT_CALL(mock_lcd, turnOff);

	on_charging_start_timeout();
}

TEST_F(RobotControllerTest, stopChargingBehavior)
{
	{
		EXPECT_CALL(mock_ledkit, stop);
		EXPECT_CALL(mock_videokit, stopVideo);
		EXPECT_CALL(mock_motor_left, stop);
		EXPECT_CALL(mock_motor_right, stop);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, stop);

	EXPECT_CALL(timeout_state_internal, stop);

	rc.stopChargingBehavior();
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsCharging)
{
	expectedCallsStopActuators();

	Sequence seq;
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection))).InSequence(seq);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, bleConnectionWithoutVideo);
	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge))).InSequence(seq);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, blinkOnCharge);

	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0);
	EXPECT_CALL(mock_lcd, turnOn).Times(0);

	rc.startConnectionBehavior();
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsNotCharging)
{
	expectedCallsStopActuators();

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection)));
		EXPECT_CALL(mock_videokit, playVideoOnce);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, bleConnectionWithVideo);

	EXPECT_CALL(mock_lcd, turnOn);

	rc.startConnectionBehavior();
}

TEST_F(RobotControllerTest, startDisconnectionBehaviorCharging)
{
	expectedCallsStopActuators();

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge))).Times(1);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, blinkOnCharge);

	rc.startDisconnectionBehavior();
}

TEST_F(RobotControllerTest, startDisconnectionBehaviorNotCharging)
{
	expectedCallsStopActuators();

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_ledkit, start).Times(0);

	rc.startDisconnectionBehavior();
}
