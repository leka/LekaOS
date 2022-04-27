// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreLCD.h"
#include "mocks/leka/CoreVideo.h"

using namespace leka;

class VideoKitTest : public ::testing::Test
{
  protected:
	VideoKitTest() {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreLCD mock_lcd {};
	mock::CoreVideo mock_corevideo {};
	VideoKit video_kit {mock_lcd, mock_corevideo};
};

TEST_F(VideoKitTest, initialization)
{
	EXPECT_NE(&video_kit, nullptr);
}

TEST_F(VideoKitTest, initializeScreen)
{
	EXPECT_CALL(mock_corevideo, initialize);
	EXPECT_CALL(mock_corevideo, setBrightness);
	EXPECT_CALL(mock_corevideo, clearScreen);

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
