// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileReception.h"

#include "mocks/leka/EventQueue.h"
#include "mocks/leka/File.h"

using namespace leka;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::Return;

MATCHER_P(compareChar, expected_char, "")
{
	return *expected_char == *arg;
}

class FileReceptionTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	FileReceptionTest() : file_reception(filemock) {}

	mock::EventQueue event_queue {};

	mock::File filemock;
	FileReception file_reception;

	static inline auto packet = std::to_array<uint8_t>({'a', 'b', 'c'});
};

TEST_F(FileReceptionTest, instantiation)
{
	EXPECT_NE(&file_reception, nullptr);
}

TEST_F(FileReceptionTest, setFilePath)
{
	EXPECT_CALL(filemock, open(Matcher<const std::filesystem::path &>(_), compareChar("w"))).WillOnce(Return(true));
	EXPECT_CALL(filemock, close).Times(1);

	file_reception.setFilePath("/fs/test.txt");
}

TEST_F(FileReceptionTest, setFilePathFileNotAccessible)
{
	EXPECT_CALL(filemock, open(Matcher<const std::filesystem::path &>(_), compareChar("w"))).WillOnce(Return(false));

	file_reception.setFilePath("/fs/unexisting_directory/test.txt");
}

TEST_F(FileReceptionTest, onPacketReceived)
{
	{
		InSequence seq;

		EXPECT_CALL(filemock, open(Matcher<const std::filesystem::path &>(_), compareChar("a"))).WillOnce(Return(true));
		EXPECT_CALL(filemock, write(Matcher<const uint8_t *>(_), std::size(packet))).Times(1);
		EXPECT_CALL(filemock, close).Times(1);
	}

	file_reception.onPacketReceived(packet);
}

TEST_F(FileReceptionTest, onPacketReceivedFileNotAccessible)
{
	EXPECT_CALL(filemock, open(Matcher<const std::filesystem::path &>(_), compareChar("a"))).WillOnce(Return(false));

	file_reception.onPacketReceived(packet);
}
