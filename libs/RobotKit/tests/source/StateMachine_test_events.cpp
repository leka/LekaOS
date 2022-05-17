// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../include/tests/StateMachine_test.h"

TEST_F(StateMachineTest, eventSetupCompleted)
{
	sm.set_current_states(lksm::state::setup);

	sm.process_event(lksm::event::setup_completed {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}

TEST_F(StateMachineTest, eventGoingToSleep)
{
	sm.set_current_states(lksm::state::idle);

	sm.process_event(lksm::event::going_to_sleep {});

	EXPECT_TRUE(sm.is(lksm::state::sleeping));
}
