// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using namespace leka::system;
namespace bsml	= boost::sml;
namespace lksm	= system::robot::sm;
namespace event = system::robot::sm::event;

class RobotControllerTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	RobotController<bsml::sm<robot::StateMachine, bsml::testing>> rc {};
};

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}

TEST_F(RobotControllerTest, stateSetupEventSetupComplete)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	rc.registerEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, startSystemAction)
{
	rc.startSystem();
}

TEST_F(RobotControllerTest, stopSystemAction)
{
	rc.stopSystem();
}
