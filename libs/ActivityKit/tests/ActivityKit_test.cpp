// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ActivityKit.h"

#include "gtest/gtest.h"
#include "mocks/Activity.h"
#include "mocks/leka/VideoKit.h"

using namespace leka;

using ::testing::AnyNumber;
using ::testing::InSequence;

class ActivityKitTest : public ::testing::Test
{
  protected:
	ActivityKitTest() = default;

	void SetUp() override { activitykit.registerActivities(activity_list); }
	// void TearDown() override {}

	mock::VideoKit mock_videokit;

	ActivityKit activitykit {mock_videokit};

	mock::Activity mock_activity_0 {};
	mock::Activity mock_activity_1 {};

	std::array<uint8_t, 18> data_FR {0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	std::array<uint8_t, 18> data_EN {0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	rfid::Tag tag_FR {.data = data_FR};
	rfid::Tag tag_EN {.data = data_EN};
	MagicCard dice_roll_FR = MagicCard(tag_FR);
	MagicCard dice_roll_EN = MagicCard(tag_EN);

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
	EXPECT_CALL(mock_activity_0, start);
	activitykit.start(MagicCard::number_0);

	EXPECT_TRUE(activitykit.isPlaying());
}

TEST_F(ActivityKitTest, isPlayingActivityStopped)
{
	EXPECT_CALL(mock_activity_0, start);
	activitykit.start(MagicCard::number_0);

	EXPECT_CALL(mock_activity_0, stop);
	activitykit.stop();

	EXPECT_FALSE(activitykit.isPlaying());
}

TEST_F(ActivityKitTest, displayENMainMenu)
{
	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-misc-choose_activity-en_US.jpg"}))
		.Times(1);

	activitykit.displayMainMenu(dice_roll_EN);
}

TEST_F(ActivityKitTest, displayFRMainMenu)
{
	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-misc-choose_activity-fr_FR.jpg"}))
		.Times(1);

	activitykit.displayMainMenu(dice_roll_FR);
}
