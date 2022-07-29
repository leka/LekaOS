// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <functional>

#include "CoreRFIDReader.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreBufferedSerial.h"
#include "mocks/leka/EventQueue.h"

using namespace leka;
using namespace interface;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArrayArgument;

class CoreRFIDReaderTest : public ::testing::Test
{
  protected:
	CoreRFIDReaderTest() : reader(mockBufferedSerial) {};

	void SetUp() override
	{
		reader.registerOnTagReadableCallback(callback_readable.AsStdFunction());
		reader.registerOnTagDetectedCallback(callback_detected.AsStdFunction());
		EXPECT_CALL(mockBufferedSerial, sigio).WillOnce(SaveArg<0>(&callback_sigio));
		reader.init();
	}
	// void TearDown() override {}

	mock::EventQueue event_queue {};
	mock::CoreBufferedSerial mockBufferedSerial;
	CoreRFIDReader reader;

	testing::MockFunction<void()> callback_detected;
	testing::MockFunction<void(rfid::Tag)> callback_readable;
	std::function<void()> callback_sigio {};

	void sendSetProtocol()
	{
		const auto expected_values = ElementsAre(rfid::command::set_protocol::id, rfid::command::set_protocol::length,
												 rfid::protocol::iso14443A.id, rfid::settings::default_rx_tx_speed);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void sendSetGainAndModulation()
	{
		const auto expected_values = ElementsAre(
			rfid::command::set_gain_and_modulation::id, rfid::command::set_gain_and_modulation::length,
			rfid::settings::arc_b, rfid::settings::flag_increment, rfid::settings::acr_b_index_for_gain_and_modulation,
			rfid::protocol::iso14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	template <size_t size>
	void receiveRFIDReaderAnswer(const std::array<uint8_t, size> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + size), Return(size)));
	}
};

TEST_F(CoreRFIDReaderTest, initialization)
{
	EXPECT_NE(&reader, nullptr);
}

TEST_F(CoreRFIDReaderTest, init)
{
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1);

	reader.init();
}

TEST_F(CoreRFIDReaderTest, registerCallbacks)
{
	reader.registerOnTagDetectedCallback(callback_detected.AsStdFunction());
	reader.registerOnTagReadableCallback(callback_readable.AsStdFunction());
}

TEST_F(CoreRFIDReaderTest, setModeTagDetection)
{
	const auto expected_values = testing::ElementsAreArray(rfid::command::frame::set_tag_detection_mode);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	reader.setModeTagDetection();
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolSuccess)
{
	std::array<uint8_t, 2> set_protocol_success_answer = {0x00, 0x00};

	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_success_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	callback_sigio();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

// TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnAnswerTooBig)
// {
// 	std::array<uint8_t, 3> set_protocol_failed_answer = {0x00, 0x00, 0x00};
// 	{
// 		InSequence seq;
// 		receiveRFIDReaderAnswer(set_protocol_failed_answer);
// 		sendSetProtocol();
// 		sendSetGainAndModulation();
// 	}

// 	callback_sigio();
// 	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
// }

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnWrongFirstValue)
{
	std::array<uint8_t, 2> set_protocol_failed_answer = {0x82, 0x00};
	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_failed_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	callback_sigio();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, sendCommandSuccess)
{
	std::array<uint8_t, 2> command = {0x26, 0x07};
	const auto expected_values	   = ElementsAre(rfid::command::send_receive, command.size(), command[0], command[1]);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	reader.sendRequestToTag(command);
}

TEST_F(CoreRFIDReaderTest, receiveDataSuccess)
{
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C};

	receiveRFIDReaderAnswer(read_values);
	callback_sigio();
	auto tag = reader.getTag();

	bool is_communication_succeed = reader.didTagCommunicationSucceed(tag.data.size());

	EXPECT_EQ(is_communication_succeed, true);
	EXPECT_TRUE(std::equal(tag.data.begin(), tag.data.end(), expected_values.begin()));
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongAnswerFlag)
{
	std::array<uint8_t, 23> read_values = {0xff, 0x05, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	receiveRFIDReaderAnswer(read_values);

	callback_sigio();

	auto tag					  = reader.getTag();
	bool is_communication_succeed = reader.didTagCommunicationSucceed(tag.data.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_FALSE(std::equal(tag.data.begin(), tag.data.end(), read_values.begin()));
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongLength)
{
	std::array<uint8_t, 7> read_values = {0x80, 0x02, 0x44, 0x00, 0x28, 0x00, 0x00};

	receiveRFIDReaderAnswer(read_values);

	callback_sigio();

	auto tag = reader.getTag();

	bool is_communication_succeed = reader.didTagCommunicationSucceed(read_values.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_FALSE(std::equal(read_values.begin(), read_values.end(), tag.data.begin()));
}

TEST_F(CoreRFIDReaderTest, getTag)
{
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C};

	receiveRFIDReaderAnswer(read_values);

	callback_sigio();

	reader.onTagReadable();

	auto tag = reader.getTag();

	EXPECT_EQ(tag.data, expected_values);
}
