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

using testing::Return;

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
	EXPECT_TRUE(sm.is(lksm::state::setup));
	EXPECT_FALSE(sm.is(lksm::state::idle));
	EXPECT_FALSE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateSetupEventSetupComplete)
{
	sm.set_current_states(lksm::state::setup);

	sm.process_event(lksm::event::setup_complete {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}

TEST_F(StateMachineTest, stateIdleEventChargeDidStart)
{
	sm.set_current_states(lksm::state::idle);

	EXPECT_CALL(mock_rc, isCharging).WillOnce(Return(true));

	sm.process_event(lksm::event::charge_did_start {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateChargingEventChargeDidStop)
{
	sm.set_current_states(lksm::state::charging);

	EXPECT_CALL(mock_rc, isCharging).WillOnce(Return(false));

	sm.process_event(lksm::event::charge_did_stop {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}
