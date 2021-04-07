// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AnimationBouncingSquare.h"

#include "gtest/gtest.h"
#include "mock_LKCoreGraphics.h"

using namespace leka;

class AnimationBouncingSquareTest : public ::testing::Test
{
  protected:
	AnimationBouncingSquareTest() : animation_bouncing_square(graphicsmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreGraphicsMock graphicsmock;
	AnimationBouncingSquare animation_bouncing_square;
};

TEST_F(AnimationBouncingSquareTest, stopAnimation)
{
	EXPECT_CALL(graphicsmock, clearScreen).Times(1);

	animation_bouncing_square.stop();
}
