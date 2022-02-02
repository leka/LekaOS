// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Battery.h"
#include "mocks/leka/Timeout.h"

using namespace leka;
using namespace leka::system;
namespace bsml	= boost::sml;
namespace lksm	= system::robot::sm;
namespace event = system::robot::sm::event;

using testing::Return;

ACTION_TEMPLATE(GetCallback, HAS_1_TEMPLATE_PARAMS(typename, callback_t), AND_1_VALUE_PARAMS(pointer))
{
	*pointer = callback_t(arg0);
}

class RobotControllerTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		EXPECT_CALL(sleep_timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_sleep_timeout));
		EXPECT_CALL(battery, onChargeDidStart).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_start));
		EXPECT_CALL(battery, onChargeDidStop).WillOnce(GetCallback<mbed::Callback<void()>>(&on_charge_did_stop));
		EXPECT_CALL(sleep_timeout, start).Times(1);	  // Hide Uninteresting mock function call

		rc.registerEvents();
	}
	// void TearDown() override {}

	mock::Timeout sleep_timeout {};
	mock::Battery battery {};

	RobotController<bsml::sm<robot::StateMachine, bsml::testing>> rc {sleep_timeout, battery};

	interface::Timeout::callback_t on_sleep_timeout {
	};

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

	EXPECT_CALL(sleep_timeout, onTimeout).Times(1);
	EXPECT_CALL(battery, onChargeDidStart).Times(1);
	EXPECT_CALL(battery, onChargeDidStop).Times(1);
	EXPECT_CALL(sleep_timeout, start).Times(1);

	rc.registerEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateIdleEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(sleep_timeout, stop).Times(1);

	on_sleep_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(sleep_timeout, stop).Times(1);

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
	EXPECT_CALL(sleep_timeout, start).Times(1);

	on_charge_did_stop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}
