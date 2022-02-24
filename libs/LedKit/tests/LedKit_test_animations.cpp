// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "events/tests/UNITTESTS/doubles/EventQueue_stub.h"
#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "LedKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/LEDAnimation.h"

using namespace leka;

using ::testing::InSequence;

class LedKitTestAnimations : public ::testing::Test
{
  protected:
	LedKitTestAnimations() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	rtos::Thread animation_thread;
	events::EventQueue event_queue;

	std::array<RGB, LedKit::kNumberOfLedsBelt> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};

	LedKit ledkit {animation_thread, event_queue, ears, belt};

	animation::LEDAnimationMock animation {};

	void MOCK_FUNCTION_silenceUnexpectedCalls() { EXPECT_CALL(animation, start); }
};

TEST_F(LedKitTestAnimations, initialization)
{
	EXPECT_NE(&animation, nullptr);
}

TEST_F(LedKitTestAnimations, startFirstAnimation)
{
	EXPECT_CALL(animation, start);

	ledkit.start(animation);
}

TEST_F(LedKitTestAnimations, stopWithoutAnimation)
{
	EXPECT_CALL(animation, stop).Times(0);

	ledkit.stop();
}

TEST_F(LedKitTestAnimations, stopStartedAnimation)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	EXPECT_CALL(animation, stop).Times(1);

	ledkit.start(animation);
	ledkit.stop();
}

TEST_F(LedKitTestAnimations, startNewAnimationSequence)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	animation::LEDAnimationMock new_animation;

	{
		InSequence seq;

		EXPECT_CALL(animation, stop);
		EXPECT_CALL(new_animation, start);
	}

	ledkit.start(animation);

	ledkit.start(new_animation);
}
