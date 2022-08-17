// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ISO14443A.h"
#include "gtest/gtest.h"

using namespace leka::rfid;

class ISO14443AUtilsTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}
};

TEST_F(ISO14443AUtilsTest, computeCRCChecksumCorrect1)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x67, 0x1A};

	Tag tag {.data = std::array<uint8_t, register_answer_size> {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
																0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xCA, 0x67}};

	EXPECT_EQ(expected_checksum, computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, computeCRCChecksumCorrect2)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x37, 0x49};

	Tag tag {.data = std::array<uint8_t, register_answer_size> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
																0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x1A}};

	EXPECT_EQ(expected_checksum, computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, computeCRCChecksumIncorrect)
{
	auto expected_checksum = std::array<uint8_t, 2> {0x49, 0x1A};

	Tag tag {.data = std::array<uint8_t, register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01,
																0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x49, 0x1A}};

	EXPECT_NE(expected_checksum, computeCRC(tag.data.data()));
}

TEST_F(ISO14443AUtilsTest, checkATQATagCorrect)
{
	Tag tag {.data = {0x44, 0x00}};

	EXPECT_TRUE(checkATQA(tag));
}

TEST_F(ISO14443AUtilsTest, checkATQATagIncorrect)
{
	Tag tag {.data = {0x00, 0x00}};

	EXPECT_FALSE(checkATQA(tag));
}

TEST_F(ISO14443AUtilsTest, checkRegisterTagCorrect)
{
	Tag tag {.data = std::array<uint8_t, register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01,
																0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x24, 0xF5}};

	EXPECT_TRUE(checkRegister(tag));
}

TEST_F(ISO14443AUtilsTest, checkRegisterTagIncorrect)
{
	Tag tag {.data = std::array<uint8_t, register_answer_size> {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01,
																0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00}};

	EXPECT_FALSE(checkRegister(tag));
}
