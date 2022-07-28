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

	// void SetUp() override {}
	// void TearDown() override {}

	mock::EventQueue event_queue {};
	mock::CoreBufferedSerial mockBufferedSerial;
	CoreRFIDReader reader;

	bool _callbackCalled {false};

	static void func1() {};
	static void func2(rfid::Tag tag) {};
	std::function<void()> callbackDetected			= func1;
	std::function<void(rfid::Tag)> callbackReadable = func2;
	std::function<void()> callbackSigio {};

	void isCallbackCalled() { _callbackCalled = true; };

	void setFakeCallback()
	{
		_callbackCalled = false;
		auto callback	= [this]() { this->isCallbackCalled(); };
		reader.registerOnTagDetectedCallback(callback);
	}

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
	reader.registerOnTagDetectedCallback(callbackDetected);
	reader.registerOnTagReadableCallback(callbackReadable);
}

TEST_F(CoreRFIDReaderTest, setModeTagDetection)
{
	const auto expected_values =
		ElementsAre(rfid::command::frame::set_tag_detection_mode[0], rfid::command::frame::set_tag_detection_mode[1],
					rfid::command::frame::set_tag_detection_mode[2], rfid::command::frame::set_tag_detection_mode[3],
					rfid::command::frame::set_tag_detection_mode[4], rfid::command::frame::set_tag_detection_mode[5],
					rfid::command::frame::set_tag_detection_mode[6], rfid::command::frame::set_tag_detection_mode[7],
					rfid::command::frame::set_tag_detection_mode[8], rfid::command::frame::set_tag_detection_mode[9],
					rfid::command::frame::set_tag_detection_mode[10], rfid::command::frame::set_tag_detection_mode[11],
					rfid::command::frame::set_tag_detection_mode[12], rfid::command::frame::set_tag_detection_mode[13],
					rfid::command::frame::set_tag_detection_mode[14], rfid::command::frame::set_tag_detection_mode[15]);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	;

	reader.setModeTagDetection();
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolSuccess)
{
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 2> set_protocol_success_answer			  = {0x00, 0x00};
	std::array<uint8_t, 2> set_gain_and_modulation_success_answer = {0x00, 0x00};

	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_success_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	callbackSigio();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnAnswerTooBig)
{
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 3> set_protocol_failed_answer = {0x00, 0x00, 0x00};
	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_failed_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	callbackSigio();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnWrongFirstValue)
{
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 2> set_protocol_failed_answer = {0x82, 0x00};
	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_failed_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	callbackSigio();
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
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C};

	receiveRFIDReaderAnswer(read_values);
	callbackSigio();
	auto tag = reader.getTag();

	bool is_communication_succeed = reader.didTagCommunicationSucceed(tag.data.size());

	EXPECT_EQ(is_communication_succeed, true);
	EXPECT_TRUE(std::equal(tag.data.begin(), tag.data.end(), expected_values.begin()));
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongAnswerFlag)
{
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 23> read_values = {0xff, 0x05, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	receiveRFIDReaderAnswer(read_values);

	callbackSigio();

	auto tag					  = reader.getTag();
	bool is_communication_succeed = reader.didTagCommunicationSucceed(tag.data.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_FALSE(std::equal(tag.data.begin(), tag.data.end(), read_values.begin()));
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongLength)
{
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 7> read_values = {0x80, 0x02, 0x44, 0x00, 0x28, 0x00, 0x00};

	receiveRFIDReaderAnswer(read_values);

	callbackSigio();

	auto tag = reader.getTag();

	bool is_communication_succeed = reader.didTagCommunicationSucceed(read_values.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_FALSE(std::equal(read_values.begin(), read_values.end(), tag.data.begin()));
}

TEST_F(CoreRFIDReaderTest, getTag)
{
	reader.registerOnTagReadableCallback(callbackReadable);
	reader.registerOnTagDetectedCallback(callbackDetected);
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1).WillOnce(SaveArg<0>(&callbackSigio));
	reader.init();

	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C};

	receiveRFIDReaderAnswer(read_values);

	callbackSigio();

	reader.onTagReadable();

	auto tag = reader.getTag();

	EXPECT_EQ(tag.data, expected_values);
}
