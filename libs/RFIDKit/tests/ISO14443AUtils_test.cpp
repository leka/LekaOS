// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ISO14443A.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreRFIDReaderCR95HF.h"

using namespace leka;
using namespace rfid::sm;

using ::testing::Return;
using ::testing::ReturnRef;

class ISO14443AUtilsTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}
};

TEST_F(ISO14443AUtilsTest, computeCRCChecksumCorrect1)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x67, 0x1A};

	rfid::Tag tag {.data = std::array<uint8_t, rfid::register_answer_size> {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
																			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
																			0x01, 0x01, 0xCA, 0x67}};

	EXPECT_EQ(expected_checksum, rfid::computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, computeCRCChecksumCorrect2)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x37, 0x49};

	rfid::Tag tag {.data = std::array<uint8_t, rfid::register_answer_size> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
																			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
																			0x00, 0x00, 0x49, 0x1A}};

	EXPECT_EQ(expected_checksum, rfid::computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, computeCRCChecksumIncorrect)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x49, 0x1A};

	rfid::Tag tag {.data = std::array<uint8_t, rfid::register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03,
																			0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
																			0x03, 0x04, 0x49, 0x1A}};

	EXPECT_NE(expected_checksum, rfid::computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, checkATQATagCorrect)
{
	rfid::Tag tag {.data = {0x44, 0x00}};

	EXPECT_TRUE(rfid::checkATQA(tag));
}

TEST_F(ISO14443AUtilsTest, checkATQATagIncorrect)
{
	rfid::Tag tag {.data = {0x00, 0x00}};

	EXPECT_FALSE(rfid::checkATQA(tag));
}

TEST_F(ISO14443AUtilsTest, checkRegisterTagCorrect)
{
	rfid::Tag tag {.data = std::array<uint8_t, rfid::register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03,
																			0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
																			0x03, 0x04, 0x24, 0xF5}};

	EXPECT_TRUE(rfid::checkRegister(tag));
}

TEST_F(ISO14443AUtilsTest, checkRegisterTagIncorrect)
{
	rfid::Tag tag {.data = std::array<uint8_t, rfid::register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03,
																			0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
																			0x03, 0x04, 0x00, 0x00}};

	EXPECT_FALSE(rfid::checkRegister(tag));
}
