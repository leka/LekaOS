// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ISO14443A.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreRFIDReader.h"

using namespace leka;
using namespace rfid::sm;

using ::testing::Return;
using ::testing::ReturnRef;

class ISO14443ATest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreRFIDReader mock_reader {};
	boost::sml::sm<rfid::ISO14443A, boost::sml::testing> sm {static_cast<interface::RFIDReader &>(mock_reader)};

	std::function<void()> magic_card_callback {};
};

inline constexpr auto command_requestA = rfid::Command<1>({.data = {0x26}, .flags = leka::rfid::Flag::sb_7});
inline constexpr auto command_read_register_4 =
	rfid::Command<2>({.data = {0x30, 0x04}, .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8});

TEST_F(ISO14443ATest, initialization)
{
	ASSERT_NE(&sm, nullptr);
}

TEST_F(ISO14443ATest, initialState)
{
	EXPECT_TRUE(sm.is(state::idle));
	EXPECT_FALSE(sm.is(state::send_reqa));
	EXPECT_FALSE(sm.is(state::requesting_atqa));
	EXPECT_FALSE(sm.is(state::requesting_tag_data));
}

TEST_F(ISO14443ATest, stateIdleEventTagResponseAvailableGuardIsTagDetectedTrue)
{
	sm.set_current_states(state::idle);

	EXPECT_CALL(mock_reader, setCommunicationProtocol).Times(1);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::send_reqa));
}

TEST_F(ISO14443ATest, stateSendReqaEventTagResponseAvailable)
{
	sm.set_current_states(state::send_reqa);

	EXPECT_CALL(mock_reader, sendRequestToTag);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::requesting_atqa));
}

TEST_F(ISO14443ATest, stateRequestingAtqaEventTagResponseAvailableGuardIsTrue)
{
	sm.set_current_states(state::requesting_atqa);

	constexpr std::array<uint8_t, rfid::ATQA_answer_size> expected_ATQA_answer {0x44, 0x00};

	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, rfid::register_answer_size> data {0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
														  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	rfid::Tag tag {id, sak, data};

	EXPECT_CALL(mock_reader, didTagCommunicationSucceed).Times(1).WillOnce(Return(true));
	EXPECT_CALL(mock_reader, getTag).WillOnce(ReturnRef(tag));
	EXPECT_CALL(mock_reader, sendRequestToTag).Times(1);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::requesting_tag_data));
}

TEST_F(ISO14443ATest, stateRequestingAtqaEventTagResponseAvailableGuardIsFalse)
{
	sm.set_current_states(state::requesting_atqa);

	EXPECT_CALL(mock_reader, didTagCommunicationSucceed).Times(2).WillRepeatedly(Return(false));
	EXPECT_CALL(mock_reader, setModeTagDetection).Times(1);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::idle));
}

TEST_F(ISO14443ATest, stateRequestingTagDataEventTagResponseAvailableGuardIsTrue)
{
	sm.set_current_states(state::requesting_tag_data);

	constexpr size_t register_answer_size = 18;

	std::array<uint8_t, 7> id {};
	std::array<uint8_t, 4> sak {};
	std::array<uint8_t, rfid::register_answer_size> data {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01,
														  0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x24, 0xF5};

	rfid::Tag tag {id, sak, data};

	EXPECT_CALL(mock_reader, didTagCommunicationSucceed).Times(1).WillOnce(Return(true));

	EXPECT_CALL(mock_reader, getTag).WillOnce(ReturnRef(tag));
	EXPECT_CALL(mock_reader, onTagReadable).Times(1);
	EXPECT_CALL(mock_reader, setModeTagDetection).Times(1);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::idle));
}

TEST_F(ISO14443ATest, stateRequestingTagDataEventTagResponseAvailableGuardIsFalse)
{
	sm.set_current_states(state::requesting_tag_data);

	EXPECT_CALL(mock_reader, didTagCommunicationSucceed).Times(2).WillRepeatedly(Return(false));
	EXPECT_CALL(mock_reader, setModeTagDetection).Times(1);

	sm.process_event(event::tag_response_available {});

	EXPECT_TRUE(sm.is(state::idle));
}
