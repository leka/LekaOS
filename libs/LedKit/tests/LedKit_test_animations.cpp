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

	mock::LEDAnimation mock_animation {};

	void MOCK_FUNCTION_silenceUnexpectedCalls() { EXPECT_CALL(mock_animation, start); }
};

TEST_F(LedKitTestAnimations, initialization)
{
	EXPECT_NE(&mock_animation, nullptr);
}

TEST_F(LedKitTestAnimations, startFirstAnimation)
{
	EXPECT_CALL(mock_animation, start);
	EXPECT_CALL(mock_animation, setLeds).Times(1);

	ledkit.start(&mock_animation);
}

TEST_F(LedKitTestAnimations, startNullPtr)
{
	EXPECT_CALL(mock_animation, setLeds).Times(0);

	ledkit.start(nullptr);
}

TEST_F(LedKitTestAnimations, stopWithoutAnimation)
{
	EXPECT_CALL(mock_animation, stop).Times(0);

	ledkit.stop();
}

TEST_F(LedKitTestAnimations, stopStartedAnimation)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	EXPECT_CALL(mock_animation, stop).Times(1);

	ledkit.start(&mock_animation);
	ledkit.stop();
}

TEST_F(LedKitTestAnimations, startNewAnimationSequence)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	mock::LEDAnimation mock_new_animation;

	{
		InSequence seq;

		EXPECT_CALL(mock_animation, stop);
		EXPECT_CALL(mock_new_animation, start);
	}

	ledkit.start(&mock_animation);

	ledkit.start(&mock_new_animation);
}
