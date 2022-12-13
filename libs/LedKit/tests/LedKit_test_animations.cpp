// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "CoreSPI.h"
#include "LedKit.h"
#include "gtest/gtest.h"
#include "mocks/Animation.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;
using namespace std::chrono;

using ::testing::InSequence;

class LedKitTestAnimations : public ::testing::Test
{
  protected:
	LedKitTestAnimations() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop {};

	std::array<RGB, LedKit::kNumberOfLedsBelt> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};

	LedKit ledkit {stub_event_loop, ears, belt};

	mock::Animation mock_animation {};
};

TEST_F(LedKitTestAnimations, initialization)
{
	EXPECT_NE(&mock_animation, nullptr);
}

TEST_F(LedKitTestAnimations, init)
{
	ledkit.init();

	std::fill(expected_colors.begin(), expected_colors.end(), RGB::black);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), belt.getColor().begin()));
}

TEST_F(LedKitTestAnimations, startAnimation)
{
	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, startCalled).Times(1);
	EXPECT_FALSE(mock_animation.isRunning());

	ledkit.start(&mock_animation);
	EXPECT_TRUE(mock_animation.isRunning());
}

TEST_F(LedKitTestAnimations, startNullPtr)
{
	EXPECT_CALL(mock_animation, setLeds).Times(0);
	EXPECT_CALL(mock_animation, startCalled).Times(0);
	EXPECT_FALSE(mock_animation.isRunning());

	ledkit.start(nullptr);
	EXPECT_FALSE(mock_animation.isRunning());
}

TEST_F(LedKitTestAnimations, runAnimation)
{
	auto kMaxStageNumber = 10;
	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, startCalled).Times(1);
	EXPECT_CALL(mock_animation, stageCalled).Times(kMaxStageNumber);

	ledkit.init();
	ledkit.start(&mock_animation);
}

TEST_F(LedKitTestAnimations, stopWithoutAnimation)
{
	EXPECT_CALL(mock_animation, stopCalled).Times(0);

	ledkit.stop();
}

TEST_F(LedKitTestAnimations, stopStartedAnimation)
{
	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, startCalled).Times(1);
	EXPECT_CALL(mock_animation, stopCalled).Times(1);
	EXPECT_FALSE(mock_animation.isRunning());

	ledkit.start(&mock_animation);
	EXPECT_TRUE(mock_animation.isRunning());

	ledkit.stop();
	EXPECT_FALSE(mock_animation.isRunning());
}

TEST_F(LedKitTestAnimations, startNewAnimationSequence)
{
	mock::Animation mock_new_animation;

	{
		InSequence seq;

		EXPECT_CALL(mock_animation, setLeds).Times(1);
		EXPECT_CALL(mock_animation, startCalled).Times(1);
		EXPECT_CALL(mock_animation, stopCalled).Times(1);
		EXPECT_CALL(mock_new_animation, setLeds).Times(1);
		EXPECT_CALL(mock_new_animation, startCalled).Times(1);
	}

	EXPECT_FALSE(mock_animation.isRunning());
	EXPECT_FALSE(mock_new_animation.isRunning());

	ledkit.start(&mock_animation);

	EXPECT_TRUE(mock_animation.isRunning());
	EXPECT_FALSE(mock_new_animation.isRunning());

	ledkit.start(&mock_new_animation);

	EXPECT_FALSE(mock_animation.isRunning());
	EXPECT_TRUE(mock_new_animation.isRunning());
}
