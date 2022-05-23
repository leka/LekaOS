// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateScreensaverAnimationDidEnd)
{
	rc.state_machine.set_current_states(lksm::state::screensaver);

	Sequence on_sleeping_sequence;
	EXPECT_CALL(mock_videokit, playVideo).InSequence(on_sleeping_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_sleeping_sequence);
	EXPECT_CALL(timeout, onTimeout)
		.InSequence(on_sleeping_sequence)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleeping_start_timeout));
	EXPECT_CALL(timeout, start).InSequence(on_sleeping_sequence);

	rc.state_machine.process_event(lksm::event::screensaver_animation_did_end {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));

	EXPECT_CALL(mock_lcd, turnOff);
	on_sleeping_start_timeout();
}
