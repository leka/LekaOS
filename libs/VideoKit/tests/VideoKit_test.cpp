// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "gtest/gtest.h"

using namespace leka;

class VideoKitTest : public ::testing::Test
{
  protected:
	VideoKitTest() {}

	// void SetUp() override {}
	// void TearDown() override {}

	VideoKit video_kit;
};

TEST_F(VideoKitTest, initialization)
{
	EXPECT_NE(&video_kit, nullptr);
}

TEST_F(VideoKitTest, turnOn)
{
	video_kit.turnOn();
}

TEST_F(VideoKitTest, turnOff)
{
	video_kit.turnOff();
}

TEST_F(VideoKitTest, initializeScreen)
{
	video_kit.initializeScreen();
}

TEST_F(VideoKitTest, displayImage)
{
	video_kit.displayImage("some_image");
}

TEST_F(VideoKitTest, playVideoInALoopFalse)
{
	auto play_in_a_loop = false;
	video_kit.playVideo("some_video", play_in_a_loop);
}

TEST_F(VideoKitTest, playVideoInALoopTrue)
{
	auto play_in_a_loop = true;
	video_kit.playVideo("some_video", play_in_a_loop);
}

TEST_F(VideoKitTest, stopVideo)
{
	video_kit.stopVideo();
}
