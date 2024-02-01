// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startIdleTimeout)
{
	auto expected_duration = 600s;

	Sequence seq;
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_idle_timeout));
	EXPECT_CALL(timeout_state_transition, start(std::chrono::microseconds {expected_duration})).InSequence(seq);

	rc.startIdleTimeout();
}

TEST_F(RobotControllerTest, onIdleTimeout)
{
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_idle_timeout));
	EXPECT_CALL(timeout_state_transition, start);
	rc.startIdleTimeout();

	on_idle_timeout();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, stopIdleTimeout)
{
	EXPECT_CALL(timeout_state_transition, stop);

	rc.stopIdleTimeout();
}

TEST_F(RobotControllerTest, startSleepTimeout)
{
	auto expected_duration = 60s;

	Sequence seq;
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));
	EXPECT_CALL(timeout_state_transition, start(std::chrono::microseconds {expected_duration})).InSequence(seq);

	rc.startSleepTimeout();
}

TEST_F(RobotControllerTest, onSleepTimeout)
{
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));
	EXPECT_CALL(timeout_state_transition, start);
	rc.startSleepTimeout();

	{
		EXPECT_CALL(timeout_state_transition, stop);
		{
			EXPECT_CALL(mock_ledkit, stop);
			EXPECT_CALL(mock_videokit, stopVideo);
			EXPECT_CALL(mock_motor_left, stop);
			EXPECT_CALL(mock_motor_right, stop);
		}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, stop);

		EXPECT_CALL(timeout_state_transition, onTimeout);
		EXPECT_CALL(timeout_state_transition, start);
		{
			{
				EXPECT_CALL(mock_videokit, playVideoOnce);
				EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::sleeping)));

			}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, sleeping);
			EXPECT_CALL(mock_lcd, turnOn);

			EXPECT_CALL(timeout_state_internal, onTimeout);
			EXPECT_CALL(timeout_state_internal, start);
		}
	}	// TODO: Expect raise call

	on_sleep_timeout();
}

TEST_F(RobotControllerTest, stopSleepTimeout)
{
	EXPECT_CALL(timeout_state_transition, stop);

	rc.stopSleepTimeout();
}

TEST_F(RobotControllerTest, startDeepSleepTimeout)
{
	auto expected_duration = 600s;

	Sequence seq;
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_deep_sleep_timeout));
	EXPECT_CALL(timeout_state_transition, start(std::chrono::microseconds {expected_duration})).InSequence(seq);

	rc.startDeepSleepTimeout();
}

TEST_F(RobotControllerTest, onDeepSleepTimeout)
{
	EXPECT_CALL(timeout_state_transition, onTimeout)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_deep_sleep_timeout));
	EXPECT_CALL(timeout_state_transition, start);
	rc.startDeepSleepTimeout();

	on_deep_sleep_timeout();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, stopDeepSleepTimeout)
{
	EXPECT_CALL(timeout_state_transition, stop);

	rc.stopDeepSleepTimeout();
}
