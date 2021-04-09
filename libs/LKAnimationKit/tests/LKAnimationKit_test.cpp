// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKAnimationKit.h"

#include "EventQueue_stub.h"
#include "Thread_stub.h"
#include "gtest/gtest.h"
#include "mock_CGAnimation.h"

using namespace leka;

using ::testing::InSequence;

class LKAnimationKitTest : public ::testing::Test
{
  protected:
	LKAnimationKitTest() : animationkit(animation_thread, event_queue, animation) {}

	// void SetUp() override {}
	// void TearDown() override {}

	rtos::Thread animation_thread;
	events::EventQueue event_queue;
	AnimationMock animation;
	LKAnimationKit animationkit;
};

TEST_F(LKAnimationKitTest, start)
{
	{
		InSequence seq;

		EXPECT_CALL(animation, stop);
		EXPECT_CALL(animation, start);
	}

	animationkit.start(animation);
}

TEST_F(LKAnimationKitTest, stop)
{
	EXPECT_CALL(animation, stop);

	animationkit.stop();
}
