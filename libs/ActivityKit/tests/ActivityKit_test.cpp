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

	// void SetUp() override {}
	// void TearDown() override {}

	ActivityKit activitykit;

	mock::Activity mock_activity {};
};

TEST_F(ActivityKitTest, initialization)
{
	EXPECT_NE(&mock_activity, nullptr);
}

TEST_F(ActivityKitTest, startactivity)
{
	EXPECT_CALL(mock_activity, start).Times(1);

	activitykit.start(&mock_activity);
}

TEST_F(ActivityKitTest, startNullPtr)
{
	EXPECT_CALL(mock_activity, start).Times(0);

	activitykit.start(nullptr);
}

TEST_F(ActivityKitTest, stopWithoutactivity)
{
	EXPECT_CALL(mock_activity, stop).Times(0);

	activitykit.stop();
}

TEST_F(ActivityKitTest, stopStartedactivity)
{
	EXPECT_CALL(mock_activity, start).Times(1);
	EXPECT_CALL(mock_activity, stop).Times(1);

	activitykit.start(&mock_activity);
	activitykit.stop();
}

TEST_F(ActivityKitTest, startNewactivitySequence)
{
	mock::Activity mock_new_activity;

	{
		InSequence seq;

		EXPECT_CALL(mock_activity, start).Times(1);
		EXPECT_CALL(mock_activity, stop).Times(1);
		EXPECT_CALL(mock_new_activity, start).Times(1);
	}

	activitykit.start(&mock_activity);

	activitykit.start(&mock_new_activity);
}
