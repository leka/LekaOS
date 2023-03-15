// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreVideo.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;

class VideoKitTest : public ::testing::Test
{
  protected:
	VideoKitTest() {}

	void SetUp() override
	{
		strcpy(temp_file_path, "/tmp/XXXXXX");
		mkstemp(temp_file_path);
	}
	// void TearDown() override {}

	char temp_file_path[L_tmpnam];	 // NOLINT

	stub::EventLoopKit stub_event_loop {};
	mock::CoreVideo mock_corevideo {};
	VideoKit video_kit {stub_event_loop, mock_corevideo};
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
	EXPECT_CALL(mock_corevideo, displayImage);

	video_kit.displayImage(temp_file_path);
}

TEST_F(VideoKitTest, displayImageFileDoesNotExist)
{
	video_kit.displayImage("/unexisting/path");
}

TEST_F(VideoKitTest, displayImageSamePathTwice)
{
	EXPECT_CALL(mock_corevideo, displayImage).Times(1);

	video_kit.displayImage(temp_file_path);

	EXPECT_CALL(mock_corevideo, displayImage).Times(0);

	video_kit.displayImage(temp_file_path);
}

TEST_F(VideoKitTest, fillWhiteBackgroundDisplayImage)
{
	EXPECT_CALL(mock_corevideo, clearScreen);
	EXPECT_CALL(mock_corevideo, displayImage);

	video_kit.fillWhiteBackgroundAndDisplayImage(temp_file_path);
}

TEST_F(VideoKitTest, fillWhiteBackgroundDisplayImageFileDoesNotExist)
{
	video_kit.fillWhiteBackgroundAndDisplayImage("/unexisting/path");
}

TEST_F(VideoKitTest, fillWhiteBackgroundDisplayImageSamePathTwice)
{
	EXPECT_CALL(mock_corevideo, clearScreen).Times(1);
	EXPECT_CALL(mock_corevideo, displayImage).Times(1);

	video_kit.fillWhiteBackgroundAndDisplayImage(temp_file_path);

	EXPECT_CALL(mock_corevideo, clearScreen).Times(0);
	EXPECT_CALL(mock_corevideo, displayImage).Times(0);

	video_kit.fillWhiteBackgroundAndDisplayImage(temp_file_path);
}

TEST_F(VideoKitTest, playVideoOnce)
{
	video_kit.playVideoOnce(temp_file_path);
}

TEST_F(VideoKitTest, playVideoOnRepeat)
{
	video_kit.playVideoOnRepeat(temp_file_path);
}

TEST_F(VideoKitTest, playVideoOnceFileDoesNotExist)
{
	video_kit.playVideoOnce("some_video");
}

TEST_F(VideoKitTest, playVideoOnRepeatFileDoesNotExist)
{
	video_kit.playVideoOnRepeat("some_video");
}

TEST_F(VideoKitTest, stopVideo)
{
	video_kit.stopVideo();
}

TEST_F(VideoKitTest, run)
{
	auto n_frames = uint8_t {99};
	{
		InSequence seq;

		EXPECT_CALL(mock_corevideo, setVideo).Times(1);

		for (auto i = 0; i < n_frames; i++) {
			EXPECT_CALL(mock_corevideo, isLastFrame).WillOnce(Return(false));
			EXPECT_CALL(mock_corevideo, displayNextFrameVideo);
		}

		EXPECT_CALL(mock_corevideo, isLastFrame).WillOnce(Return(true));
	}

	video_kit.run();
}

TEST_F(VideoKitTest, runVideoEndedCallback)
{
	auto dummy_function = MockFunction<void()> {};
	video_kit.playVideoOnce(temp_file_path, dummy_function.AsStdFunction());

	auto n_frames = uint8_t {99};
	{
		InSequence seq;

		EXPECT_CALL(mock_corevideo, setVideo).Times(1);

		for (auto i = 0; i < n_frames; i++) {
			EXPECT_CALL(mock_corevideo, isLastFrame).WillOnce(Return(false));
			EXPECT_CALL(mock_corevideo, displayNextFrameVideo);
		}

		EXPECT_CALL(mock_corevideo, isLastFrame).WillOnce(Return(true));
		EXPECT_CALL(dummy_function, Call).Times(1);
	}

	video_kit.run();
}
