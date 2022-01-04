// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "StateMachine.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
namespace bsml = boost::sml;

struct MockStateMachineManager : public interface::StateMachineManager {
  public:
	MOCK_METHOD(void, startSystem, (), (override));
	MOCK_METHOD(void, stopSystem, (), (override));
};

class StateMachineTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	MockStateMachineManager mock_smm {};
	boost::sml::sm<StateMachine, boost::sml::testing> sm {static_cast<interface::StateMachineManager &>(mock_smm)};
};

using namespace bsml;	// ? do not move as it won't compile

TEST_F(StateMachineTest, initialization)
{
	EXPECT_NE(&sm, nullptr);
}

TEST_F(StateMachineTest, initialState)
{
	EXPECT_TRUE(sm.is("idle"_s));
	EXPECT_FALSE(sm.is("running"_s));
}

TEST_F(StateMachineTest, statieIdleEventStart)
{
	sm.set_current_states("idle"_s);

	EXPECT_CALL(mock_smm, startSystem()).Times(1);

	sm.process_event(leka::sml::event::start {});

	EXPECT_TRUE(sm.is("running"_s));
}

TEST_F(StateMachineTest, stateRunningEventTimeout)
{
	sm.set_current_states("running"_s);

	EXPECT_CALL(mock_smm, stopSystem()).Times(1);

	sm.process_event(leka::sml::event::timeout {});

	EXPECT_TRUE(sm.is("idle"_s));
}
