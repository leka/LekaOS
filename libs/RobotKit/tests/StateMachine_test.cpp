// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "StateMachine.h"

#include "CoreMutex.h"
#include "RCLogger.h"
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
	void SetUp() override { logger::set_sink_function(spy_sink_function); }
	// void TearDown() override {}

	static void spy_sink_function(const char *str, size_t size)
	{
		auto output = std::string {str};
		std::cout << output;
	}

	mock::RobotController mock_rc {};
	robot::sm::logger logger {};

	boost::sml::sm<robot::StateMachine, boost::sml::testing, boost::sml::logger<robot::sm::logger>,
				   boost::sml::thread_safe<CoreMutex>>
		sm {static_cast<interface::RobotController &>(mock_rc), logger};
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
	EXPECT_FALSE(sm.is(lksm::state::sleeping));
	EXPECT_FALSE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateSetupEventSetupCompleteGuardIsChargingFalse)
{
	sm.set_current_states(lksm::state::setup);

	EXPECT_CALL(mock_rc, runLaunchingBehavior).Times(1);
	EXPECT_CALL(mock_rc, isCharging).WillRepeatedly(Return(false));
	EXPECT_CALL(mock_rc, startSleepTimeout).Times(1);
	EXPECT_CALL(mock_rc, startWaitingBehavior).Times(1);

	sm.process_event(lksm::event::setup_complete {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}

TEST_F(StateMachineTest, stateSetupEventSetupCompleteGuardIsChargingTrue)
{
	sm.set_current_states(lksm::state::setup);

	EXPECT_CALL(mock_rc, runLaunchingBehavior).Times(1);
	EXPECT_CALL(mock_rc, isCharging).WillRepeatedly(Return(true));
	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);

	sm.process_event(lksm::event::setup_complete {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateIdleEventTimeout)
{
	sm.set_current_states(lksm::state::idle);

	EXPECT_CALL(mock_rc, stopSleepTimeout).Times(1);
	EXPECT_CALL(mock_rc, stopWaitingBehavior).Times(1);
	EXPECT_CALL(mock_rc, startScreensaverBehavior).Times(1);

	sm.process_event(lksm::event::sleep_timeout_did_end {});

	EXPECT_TRUE(sm.is(lksm::state::screensaver));
}

TEST_F(StateMachineTest, stateScreensaverAnimationDidEnd)
{
	sm.set_current_states(lksm::state::screensaver);
	EXPECT_CALL(mock_rc, startSleepingBehavior).Times(1);

	sm.process_event(lksm::event::screensaver_animation_did_end {});

	EXPECT_TRUE(sm.is(lksm::state::sleeping));
}

TEST_F(StateMachineTest, stateSleepEventCommandReceived)
{
	sm.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(mock_rc, stopSleepingBehavior).Times(1);
	EXPECT_CALL(mock_rc, startSleepTimeout).Times(1);
	EXPECT_CALL(mock_rc, startWaitingBehavior).Times(1);

	sm.process_event(lksm::event::command_received {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}

TEST_F(StateMachineTest, stateSleepEventChargeDidStart)
{
	sm.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(mock_rc, stopSleepingBehavior).Times(1);
	EXPECT_CALL(mock_rc, isCharging).WillOnce(Return(true));
	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);

	sm.process_event(lksm::event::charge_did_start {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateIdleEventChargeDidStart)
{
	sm.set_current_states(lksm::state::idle);

	EXPECT_CALL(mock_rc, isCharging).WillOnce(Return(true));
	EXPECT_CALL(mock_rc, stopSleepTimeout).Times(1);
	EXPECT_CALL(mock_rc, stopWaitingBehavior).Times(1);
	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);

	sm.process_event(lksm::event::charge_did_start {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateChargingEventChargeDidStop)
{
	sm.set_current_states(lksm::state::charging);

	EXPECT_CALL(mock_rc, isCharging).WillOnce(Return(false));
	EXPECT_CALL(mock_rc, startSleepTimeout).Times(1);
	EXPECT_CALL(mock_rc, startWaitingBehavior).Times(1);
	EXPECT_CALL(mock_rc, stopChargingBehavior).Times(1);

	sm.process_event(lksm::event::charge_did_stop {});

	EXPECT_TRUE(sm.is(lksm::state::idle));
}

TEST_F(StateMachineTest, stateChargingEventUpdateRequestedGuardTrue)
{
	sm.set_current_states(lksm::state::charging);

	EXPECT_CALL(mock_rc, stopChargingBehavior).Times(1);
	EXPECT_CALL(mock_rc, isReadyToUpdate).WillOnce(Return(true));
	EXPECT_CALL(mock_rc, applyUpdate).Times(1);

	sm.process_event(lksm::event::update_requested {});

	EXPECT_TRUE(sm.is(lksm::state::updating));
}

TEST_F(StateMachineTest, stateChargingEventUpdateRequestedGuardFalse)
{
	sm.set_current_states(lksm::state::charging);

	EXPECT_CALL(mock_rc, isReadyToUpdate).WillOnce(Return(false));
	EXPECT_CALL(mock_rc, applyUpdate).Times(0);

	sm.process_event(lksm::event::update_requested {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}

TEST_F(StateMachineTest, stateSleepingEventBleConnection)
{
	sm.set_current_states(lksm::state::sleeping, lksm::state::disconnected);

	EXPECT_CALL(mock_rc, startConnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(sm.is(lksm::state::idle, lksm::state::connected));
}

TEST_F(StateMachineTest, stateSleepingEventBleDisconnection)
{
	sm.set_current_states(lksm::state::sleeping, lksm::state::connected);

	EXPECT_CALL(mock_rc, startDisconnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_disconnection {});

	EXPECT_TRUE(sm.is(lksm::state::sleeping, lksm::state::disconnected));
}

TEST_F(StateMachineTest, stateIdleEventBleConnection)
{
	sm.set_current_states(lksm::state::idle, lksm::state::disconnected);

	EXPECT_CALL(mock_rc, startConnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(sm.is(lksm::state::idle, lksm::state::connected));
}

TEST_F(StateMachineTest, stateIdleEventBleDisconnection)
{
	sm.set_current_states(lksm::state::idle, lksm::state::connected);

	EXPECT_CALL(mock_rc, startDisconnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_disconnection {});

	EXPECT_TRUE(sm.is(lksm::state::idle, lksm::state::disconnected));
}

TEST_F(StateMachineTest, stateChargingEventBleConnection)
{
	sm.set_current_states(lksm::state::charging, lksm::state::disconnected);

	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);
	EXPECT_CALL(mock_rc, isCharging).WillRepeatedly(Return(true));
	EXPECT_CALL(mock_rc, startConnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(sm.is(lksm::state::charging, lksm::state::connected));
}

TEST_F(StateMachineTest, stateChargingEventBleDisconnection)
{
	sm.set_current_states(lksm::state::charging, lksm::state::connected);

	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);
	EXPECT_CALL(mock_rc, startDisconnectionBehavior).Times(1);

	sm.process_event(lksm::event::ble_disconnection {});

	EXPECT_TRUE(sm.is(lksm::state::charging, lksm::state::disconnected));
}

TEST_F(StateMachineTest, stateChargingEventCommandReceived)
{
	sm.set_current_states(lksm::state::charging);

	EXPECT_CALL(mock_rc, startChargingBehavior).Times(1);

	sm.process_event(lksm::event::command_received {});

	EXPECT_TRUE(sm.is(lksm::state::charging));
}
