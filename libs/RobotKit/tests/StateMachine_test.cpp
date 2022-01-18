// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "StateMachine.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "interface/RobotController.h"
#include "mocks/RobotController.h"

using namespace leka;
using namespace leka::system;
namespace lksm = leka::system::robot::sm;
namespace bsml = boost::sml;

class StateMachineTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::RobotController mock_rc {};
	boost::sml::sm<robot::StateMachine, boost::sml::testing> sm {static_cast<interface::RobotController &>(mock_rc)};
};

using namespace bsml;	// ? do not move as it won't compile

TEST_F(StateMachineTest, initialization)
{
	EXPECT_NE(&sm, nullptr);
}

TEST_F(StateMachineTest, initialState)
{
	EXPECT_TRUE(sm.is(lksm::state::sleeping));
	EXPECT_FALSE(sm.is(lksm::state::waiting_for_commands));
}

TEST_F(StateMachineTest, stateSleepingEventWakeup)
{
	sm.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(mock_rc, wakeupSystem()).Times(1);
	EXPECT_CALL(mock_rc, onEntryWaitingForCommands()).Times(1);

	sm.process_event(lksm::event::wakeup {});

	EXPECT_TRUE(sm.is(lksm::state::waiting_for_commands));
}

TEST_F(StateMachineTest, stateWaitingForCommandsEventTimeout)
{
	sm.set_current_states(lksm::state::waiting_for_commands);

	EXPECT_CALL(mock_rc, fallAsleepSystem()).Times(1);

	sm.process_event(lksm::event::timeout {});

	EXPECT_TRUE(sm.is(lksm::state::sleeping));
}
