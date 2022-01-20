// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RobotController.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

class RobotControllerTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	RobotController rc {};
};

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}
