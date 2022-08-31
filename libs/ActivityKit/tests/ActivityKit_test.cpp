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

	mock::Activity mock_activity {};
	mock::Activity mock_new_activity {};

	std::unordered_map<uint16_t, interface::Activity *> activity_list = {
		{MagicCard::number_0.getId(), &mock_activity},
		{MagicCard::number_1.getId(), &mock_new_activity},
	};
};

TEST_F(ActivityKitTest, initialization)
{
	EXPECT_NE(&mock_activity, nullptr);
}

TEST_F(ActivityKitTest, startActivity)
{
	EXPECT_CALL(mock_activity, start).Times(1);

	activitykit.start(MagicCard::number_0);
}

TEST_F(ActivityKitTest, startNullPtr)
{
	EXPECT_CALL(mock_activity, start).Times(0);

	activitykit.start(MagicCard::none);
}

TEST_F(ActivityKitTest, stopWithoutActivity)
{
	EXPECT_CALL(mock_activity, stop).Times(0);

	activitykit.stop();
}

TEST_F(ActivityKitTest, stopStartedActivity)
{
	EXPECT_CALL(mock_activity, start).Times(1);
	EXPECT_CALL(mock_activity, stop).Times(1);

	activitykit.start(MagicCard::number_0);
	activitykit.stop();
}

TEST_F(ActivityKitTest, startNewActivitySequence)
{
	{
		InSequence seq;

		EXPECT_CALL(mock_activity, start).Times(1);
		EXPECT_CALL(mock_activity, stop).Times(1);
		EXPECT_CALL(mock_new_activity, start).Times(1);
	}

	activitykit.start(MagicCard::number_0);

	activitykit.start(MagicCard::number_1);
}
