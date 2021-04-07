// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKAnimationKit.h"

// #include "CGAnimation.h"
#include "Thread_stub.h"
#include "gtest/gtest.h"
#include "mock_CGAnimation.h"

using namespace leka;

class LKAnimationKitTest : public ::testing::Test
{
  protected:
	LKAnimationKitTest() : animationkit(animation_thread, animation) {}

	// void SetUp() override {}
	// void TearDown() override {}

	rtos::Thread animation_thread;
	AnimationMock animation;
	LKAnimationKit animationkit;
};

TEST_F(LKAnimationKitTest, instanciation) {}

TEST_F(LKAnimationKitTest, stop)
{
	EXPECT_CALL(animation, stop);

	animationkit.stop();
}
