// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::memory;

TEST(MemoryUtilsTest, getLowByte)
{
	auto value = uint16_t {};
	auto low   = uint8_t {};

	value = uint16_t {0b0000'0000'0000'0000};
	low	  = getLowByte(value);

	ASSERT_EQ(0b0000'0000, low);

	value = uint16_t {0b0000'0000'0000'0001};
	low	  = getLowByte(value);

	ASSERT_EQ(0b0000'0001, low);

	value = uint16_t {0b0000'0000'0000'1000};
	low	  = getLowByte(value);

	ASSERT_EQ(0b0000'1000, low);

	value = uint16_t {0b0000'0000'0001'0000};
	low	  = getLowByte(value);

	ASSERT_EQ(0b0001'0000, low);

	value = uint16_t {0b0000'0000'1000'0000};
	low	  = getLowByte(value);

	ASSERT_EQ(0b1000'0000, low);

	value = uint16_t {0b0000'0000'1111'0000};
	low	  = getLowByte(value);

	ASSERT_EQ(0b1111'0000, low);

	value = uint16_t {0b0000'0000'0000'1111};
	low	  = getLowByte(value);

	ASSERT_EQ(0b0000'1111, low);

	value = uint16_t {0b0000'0000'1111'1111};
	low	  = getLowByte(value);

	ASSERT_EQ(0b1111'1111, low);
}

TEST(MemoryUtilsTest, getHighByte)
{
	auto value = uint16_t {};
	auto high  = uint8_t {};

	value = uint16_t {0b0000'0000'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b0000'0000, high);

	value = uint16_t {0b0000'0001'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b0000'0001, high);

	value = uint16_t {0b0000'1000'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b0000'1000, high);

	value = uint16_t {0b0001'0000'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b0001'0000, high);

	value = uint16_t {0b1000'0000'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b1000'0000, high);

	value = uint16_t {0b1111'0000'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b1111'0000, high);

	value = uint16_t {0b0000'1111'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b0000'1111, high);

	value = uint16_t {0b1111'1111'0000'0000};
	high  = getHighByte(value);

	ASSERT_EQ(0b1111'1111, high);
}

TEST(MemoryUtilsTest, combineTwoBytes)
{
	auto high = uint8_t {0xAB};
	auto low  = uint8_t {0xCD};

	auto both = combineBytes({.high = high, .low = low});

	EXPECT_EQ(both, 0xABCD);

	auto new_high = getHighByte(both);
	auto new_low  = getLowByte(both);

	EXPECT_EQ(new_high, high);
	EXPECT_EQ(new_low, low);
}
