// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using namespace leka::system;
namespace bsml = boost::sml;

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

TEST_F(RobotControllerTest, startSystemAction)
{
	rc.startSystem();
}

TEST_F(RobotControllerTest, stopSystemAction)
{
	rc.stopSystem();
}
