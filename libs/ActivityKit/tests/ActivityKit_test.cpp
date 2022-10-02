// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ActivityKit.h"

#include "gtest/gtest.h"
#include "mocks/Activity.h"

using namespace leka;

using ::testing::InSequence;

class ActivityKitTest : public ::testing::Test
{
  protected:
	ActivityKitTest() = default;

	void SetUp() override { activitykit.registerActivities(activity_list); }
	// void TearDown() override {}

	ActivityKit activitykit;

	mock::Activity mock_activity_0 {};
	mock::Activity mock_activity_1 {};

	std::unordered_map<MagicCard, interface::Activity *> activity_list = {
		{MagicCard::number_0, &mock_activity_0},
		{MagicCard::number_1, &mock_activity_1},
	};
};

TEST_F(ActivityKitTest, initialization)
{
	EXPECT_NE(&mock_activity_0, nullptr);
}

TEST_F(ActivityKitTest, startActivity)
{
	EXPECT_CALL(mock_activity_0, start).Times(1);

	activitykit.start(MagicCard::number_0);
}

TEST_F(ActivityKitTest, startNullPtr)
{
	EXPECT_CALL(mock_activity_0, start).Times(0);

	activitykit.start(MagicCard::none);
}

TEST_F(ActivityKitTest, stopWithoutActivity)
{
	EXPECT_CALL(mock_activity_0, stop).Times(0);

	activitykit.stop();
}

TEST_F(ActivityKitTest, stopStartedActivity)
{
	EXPECT_CALL(mock_activity_0, start).Times(1);
	EXPECT_CALL(mock_activity_0, stop).Times(1);

	activitykit.start(MagicCard::number_0);
	activitykit.stop();
}

TEST_F(ActivityKitTest, startNewActivitySequence)
{
	{
		InSequence seq;

		EXPECT_CALL(mock_activity_0, start).Times(1);
		EXPECT_CALL(mock_activity_0, stop).Times(1);
		EXPECT_CALL(mock_activity_1, start).Times(1);
	}

	activitykit.start(MagicCard::number_0);

	activitykit.start(MagicCard::number_1);
}

TEST_F(ActivityKitTest, isPlayingActivityNullPtr)
{
	EXPECT_FALSE(activitykit.isPlaying());
}

TEST_F(ActivityKitTest, isPlayingActivityStarted)
{
	activitykit.start(MagicCard::number_0);

	EXPECT_TRUE(activitykit.isPlaying());
}

TEST_F(ActivityKitTest, isPlayingActivityStopped)
{
	activitykit.start(MagicCard::number_0);
	activitykit.stop();

	EXPECT_FALSE(activitykit.isPlaying());
}
