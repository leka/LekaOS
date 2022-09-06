// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MagicCard.h"

#include "gtest/gtest.h"

using namespace leka;

class MagicCardTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
								  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	rfid::Tag tag {id, sak, data};
	static constexpr uint16_t number_ten_id = 0x00'14;

	MagicCard pre_implemented_number_10 = MagicCard::number_10;
	MagicCard number_10_by_tag			= MagicCard(tag);
	MagicCard number_10_by_id			= MagicCard(number_ten_id);
};

TEST_F(MagicCardTest, initialization)
{
	ASSERT_NE(&pre_implemented_number_10, nullptr);
}

TEST_F(MagicCardTest, PreImplementedEqualToById)
{
	EXPECT_EQ(pre_implemented_number_10, number_10_by_id);
}

TEST_F(MagicCardTest, PreImplementedEqualToByTag)
{
	EXPECT_EQ(pre_implemented_number_10, number_10_by_tag);
}

TEST_F(MagicCardTest, getId)
{
	EXPECT_EQ(pre_implemented_number_10.getId(), number_ten_id);
}

TEST_F(MagicCardTest, operatorInt)
{
	auto expected_id		 = MagicCard::emergency_stop.getId();
	auto card_to_get_id_from = MagicCard::emergency_stop;

	EXPECT_EQ(expected_id, card_to_get_id_from);
}
