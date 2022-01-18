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
	EXPECT_TRUE(sm.is(lksm::state::idle));
	EXPECT_FALSE(sm.is(lksm::state::running));
}

TEST_F(StateMachineTest, stateIdleEventStart)
{
	sm.set_current_states(lksm::state::idle);

	EXPECT_CALL(mock_rc, startSystem()).Times(1);
	EXPECT_CALL(mock_rc, onRunningEntry()).Times(1);

	sm.process_event(lksm::event::start {});

	EXPECT_TRUE(sm.is(lksm::state::running));
}

TEST_F(StateMachineTest, stateRunningEventTimeout)
{
	sm.set_current_states(lksm::state::running);

	EXPECT_CALL(mock_rc, stopSystem()).Times(1);

	sm.process_event(lksm::event::timeout {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}
