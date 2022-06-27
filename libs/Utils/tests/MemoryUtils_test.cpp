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

TEST(MemoryUtilsTest, getByteAndMask)
{
	auto value = uint8_t {};
	auto mask  = uint8_t {};
	auto final = uint8_t {};

	mask  = uint8_t {0b1111'1111};
	value = uint8_t {0b0000'0000};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0000, final);

	mask  = uint8_t {0b1111'1111};
	value = uint8_t {0b0000'0001};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0001, final);

	mask  = uint8_t {0b0000'1111};
	value = uint8_t {0b0000'1000};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'1000, final);

	mask  = uint8_t {0b1111'0000};
	value = uint8_t {0b0000'0001};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0000, final);

	mask  = uint8_t {0b1100'0011};
	value = uint8_t {0b0000'1000};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0000, final);

	mask  = uint8_t {0b1111'0011};
	value = uint8_t {0b0000'1111};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0011, final);

	mask  = uint8_t {0b0000'0000};
	value = uint8_t {0b0000'1111};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b0000'0000, final);

	mask  = uint8_t {0b1100'0011};
	value = uint8_t {0b1111'1111};
	final = getByteAndMask(value, mask);

	ASSERT_EQ(0b1100'0011, final);
}

TEST(MemoryUtilsTest, getByteOrMask)
{
	auto value = uint8_t {};
	auto mask  = uint8_t {};
	auto final = uint8_t {};

	mask  = uint8_t {0b1111'1111};
	value = uint8_t {0b0000'0000};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1111'1111, final);

	mask  = uint8_t {0b1111'1111};
	value = uint8_t {0b0000'0001};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1111'1111, final);

	mask  = uint8_t {0b0000'1111};
	value = uint8_t {0b0000'1000};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b0000'1111, final);

	mask  = uint8_t {0b1111'0000};
	value = uint8_t {0b0000'0001};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1111'0001, final);

	mask  = uint8_t {0b1100'0011};
	value = uint8_t {0b0000'1000};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1100'1011, final);

	mask  = uint8_t {0b1111'0011};
	value = uint8_t {0b0000'1111};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1111'1111, final);

	mask  = uint8_t {0b0000'0000};
	value = uint8_t {0b0000'1111};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b0000'1111, final);

	mask  = uint8_t {0b1100'0011};
	value = uint8_t {0b1111'1111};
	final = getByteOrMask(value, mask);

	ASSERT_EQ(0b1111'1111, final);
}

TEST(MemoryUtilsTest, combineBytesParts)
{
	auto first_value  = uint8_t {};
	auto second_value = uint8_t {};
	auto mask		  = uint8_t {};
	auto final		  = uint8_t {};

	mask		 = uint8_t {0b1111'0000};
	first_value	 = uint8_t {0b0001'0010};
	second_value = uint8_t {0b0100'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0001'0001, final);

	mask		 = uint8_t {0b1111'0000};
	first_value	 = uint8_t {0b0000'0001};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'0001, final);

	mask		 = uint8_t {0b0000'1111};
	first_value	 = uint8_t {0b0000'1000};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'1000, final);

	mask		 = uint8_t {0b1111'0000};
	first_value	 = uint8_t {0b0000'0001};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'0001, final);

	mask		 = uint8_t {0b1100'0011};
	first_value	 = uint8_t {0b0000'1000};
	second_value = uint8_t {0b0000'0100};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'0100, final);

	mask		 = uint8_t {0b1111'0011};
	first_value	 = uint8_t {0b0000'1111};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'0011, final);

	mask		 = uint8_t {0b0000'0000};
	first_value	 = uint8_t {0b0000'1111};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b0000'0001, final);

	mask		 = uint8_t {0b1100'0011};
	first_value	 = uint8_t {0b1111'1111};
	second_value = uint8_t {0b0000'0001};

	final = combineBytesParts(first_value, second_value, mask);

	ASSERT_EQ(0b1100'0011, final);
}
