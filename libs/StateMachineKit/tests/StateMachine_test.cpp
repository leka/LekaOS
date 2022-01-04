// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "StateMachine.h"

#include "gtest/gtest.h"
#include "stubs/mbed/mbed_critical.h"

using namespace leka;

class StateMachineTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	StateMachine sm {};
};

TEST_F(StateMachineTest, initialization)
{
	EXPECT_NE(&sm, nullptr);
}
