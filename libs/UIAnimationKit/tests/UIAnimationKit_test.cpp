// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "UIAnimationKit.h"

#include "events/tests/UNITTESTS/doubles/EventQueue_stub.h"
#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "gtest/gtest.h"
#include "mocks/leka/CGAnimation.h"

using namespace leka;

using ::testing::InSequence;

class UIAnimationKitTest : public ::testing::Test
{
  protected:
	UIAnimationKitTest() : animationkit(animation_thread, event_queue) {}

	// void SetUp() override {}
	// void TearDown() override {}

	rtos::Thread animation_thread;
	events::EventQueue event_queue;
	UIAnimationKit animationkit;

	mock::CGAnimation animation;

	// TODO: These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO: Remove them in the future
	void MOCK_FUNCTION_silenceUnexpectedCalls(void) { EXPECT_CALL(animation, start); }
};

TEST_F(UIAnimationKitTest, startFirstAnimation)
{
	EXPECT_CALL(animation, start);

	animationkit.start(animation);
}

TEST_F(UIAnimationKitTest, startNewAnimationSequence)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	mock::CGAnimation new_animation;

	{
		InSequence seq;

		EXPECT_CALL(animation, stop);
		EXPECT_CALL(new_animation, start);
	}

	animationkit.start(animation);

	animationkit.start(new_animation);
}

TEST_F(UIAnimationKitTest, stopWithoutAnimation)
{
	EXPECT_CALL(animation, stop).Times(0);

	animationkit.stop();
}

TEST_F(UIAnimationKitTest, stopStartedAnimation)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	EXPECT_CALL(animation, stop).Times(1);

	animationkit.start(animation);
	animationkit.stop();
}
