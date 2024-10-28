// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"
#include "commands/VideoCommand.h"
#include "gtest/gtest.h"
#include "mocks/leka/VideoKit.h"

using namespace leka;

class VideoCommandTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	VideoCommand video_command {mock_videokit};
};

TEST_F(VideoCommandTest, initialization)
{
	EXPECT_NE(&video_command, nullptr);
}

TEST_F(VideoCommandTest, commandID)
{
	auto expected_cmd_id = 0x80;
	auto actual_cmd_id	 = video_command.id();

	EXPECT_EQ(actual_cmd_id, expected_cmd_id);
}

TEST_F(VideoCommandTest, commandSize)
{
	auto expected_cmd_size = uint8_t {2 + 1};	// image_id + Checksum
	auto actual_cmd_size   = video_command.size();

	EXPECT_EQ(actual_cmd_size, expected_cmd_size);
}

TEST_F(VideoCommandTest, execute)
{
	auto command	   = std::to_array({0x00, 0x7C, 0x7C});
	auto expected_path = std::filesystem::path("/fs/home/img/id/007C.jpg");

	auto *command_data = video_command.data();
	std::copy(command.begin(), command.end(), command_data);

	EXPECT_CALL(mock_videokit, fillWhiteBackgroundAndDisplayImage(expected_path));
	video_command.execute();
}

TEST_F(VideoCommandTest, executeIncorrectChecksum)
{
	auto command = std::to_array({0x00, 0x7C, 0x22});

	auto *command_data = video_command.data();
	std::copy(command.begin(), command.end(), command_data);

	EXPECT_CALL(mock_videokit, fillWhiteBackgroundAndDisplayImage).Times(0);
	video_command.execute();
}
