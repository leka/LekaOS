// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreRFIDReader.h"

using namespace leka;

using ::testing::MockFunction;
using ::testing::ReturnRef;
using ::testing::SaveArg;

class RFIDKitTest : public ::testing::Test
{
  protected:
	RFIDKitTest() : rfid_kit(mock_reader) {};

	// void SetUp() override {}
	// void TearDown() override {}

	RFIDKit rfid_kit;
	mock::CoreRFIDReader mock_reader {};
	MockFunction<void(MagicCard &)> mock_callback;

	std::function<void(rfid::Tag &)> magic_card_callback {};
};

TEST_F(RFIDKitTest, initialization)
{
	ASSERT_NE(&rfid_kit, nullptr);
}

TEST_F(RFIDKitTest, init)
{
	EXPECT_CALL(mock_reader, registerOnTagDetectedCallback).Times(1);
	EXPECT_CALL(mock_reader, registerOnTagReadableCallback).Times(1);
	EXPECT_CALL(mock_reader, init).Times(1);
	EXPECT_CALL(mock_reader, setModeTagDetection).Times(1);

	rfid_kit.init();
}

TEST_F(RFIDKitTest, registerMagicCardCallbackTagValidCallbackSet)
{
	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x4C, 0x45, 0x4B, 0x41, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	rfid::Tag tag_valid {id, sak, data};

	rfid_kit.onTagActivated(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_reader, registerOnTagReadableCallback).WillOnce(SaveArg<0>(&magic_card_callback));

	rfid_kit.registerMagicCard();

	EXPECT_CALL(mock_callback, Call).Times(1);
	magic_card_callback(tag_valid);
}

TEST_F(RFIDKitTest, registerMagicCardCallbackTagNotValidCallbackSet)
{
	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x4C, 0x45, 0x4B, 0x00, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	rfid::Tag tag_not_valid {id, sak, data};

	rfid_kit.onTagActivated(mock_callback.AsStdFunction());
	EXPECT_CALL(mock_reader, registerOnTagReadableCallback).WillOnce(SaveArg<0>(&magic_card_callback));

	rfid_kit.registerMagicCard();

	EXPECT_CALL(mock_callback, Call).Times(0);
	magic_card_callback(tag_not_valid);
}

TEST_F(RFIDKitTest, registerMagicCardCallbackTagValidCallbackNotSet)
{
	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x4C, 0x45, 0x4B, 0x41, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	rfid::Tag tag_valid {id, sak, data};

	rfid_kit.onTagActivated(nullptr);

	EXPECT_CALL(mock_reader, registerOnTagReadableCallback).WillOnce(SaveArg<0>(&magic_card_callback));

	rfid_kit.registerMagicCard();

	magic_card_callback(tag_valid);
}

TEST_F(RFIDKitTest, TagSignatureIsValid)
{
	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x4C, 0x45, 0x4B, 0x41, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	rfid::Tag tag_valid {id, sak, data};

	auto is_tag_valid = rfid_kit.isTagSignatureValid(tag_valid);

	EXPECT_TRUE(is_tag_valid);
}

TEST_F(RFIDKitTest, TagSignatureIsNotValid)
{
	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, 18> data {0x00, 0x45, 0x00, 0x41, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	rfid::Tag tag_not_valid {id, sak, data};

	auto is_tag_valid = rfid_kit.isTagSignatureValid(tag_not_valid);

	EXPECT_FALSE(is_tag_valid);
}

TEST_F(RFIDKitTest, onTagActivated)
{
	rfid_kit.onTagActivated(nullptr);
}
