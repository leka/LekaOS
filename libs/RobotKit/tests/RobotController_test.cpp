// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"

using namespace leka;
using namespace leka::system;
namespace bsml	= boost::sml;
namespace lksm	= system::robot::sm;
namespace event = system::robot::sm::event;

using testing::Return;

ACTION_P(GetCallback, pointer)
{
	*pointer = mbed::Callback<void()>(arg0);
}

class RobotControllerTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		EXPECT_CALL(battery, onChargeDidStart).WillOnce(GetCallback(&on_charge_did_start));
		EXPECT_CALL(battery, onChargeDidStop).WillOnce(GetCallback(&on_charge_did_stop));

		rc.registerEvents();
	}
	// void TearDown() override {}

	mock::Battery battery {};
	RobotController<bsml::sm<robot::StateMachine, bsml::testing>> rc {battery};

	mbed::Callback<void()> on_charge_did_start {};
	mbed::Callback<void()> on_charge_did_stop {};
};

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}

TEST_F(RobotControllerTest, stateSetupEventSetupComplete)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	EXPECT_CALL(battery, onChargeDidStart).Times(1);
	EXPECT_CALL(battery, onChargeDidStop).Times(1);

	rc.registerEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateChargingEventChargeDidStopGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::charging);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}
