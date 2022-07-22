// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <functional>

#include "CoreRFIDReader.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreBufferedSerial.h"

using namespace leka;
using namespace interface;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreRFIDReaderTest : public ::testing::Test
{
  protected:
	CoreRFIDReaderTest() : reader(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreBufferedSerial mockBufferedSerial;
	CoreRFIDReader reader;

	bool _callbackCalled {false};

	static void func() {};
	std::function<void()> callbackFunction = func;

	void isCallbackCalled() { _callbackCalled = true; };

	void setFakeCallback()
	{
		_callbackCalled = false;
		auto callback	= [this]() { this->isCallbackCalled(); };
		reader.registerOnTagDetectedCallback(callback);
	}

	void sendSetModeTagDetection()
	{
		const auto expected_values = ElementsAre(
			rfid::command::frame::set_tag_detection_mode[0], rfid::command::frame::set_tag_detection_mode[1],
			rfid::command::frame::set_tag_detection_mode[2], rfid::command::frame::set_tag_detection_mode[3],
			rfid::command::frame::set_tag_detection_mode[4], rfid::command::frame::set_tag_detection_mode[5],
			rfid::command::frame::set_tag_detection_mode[6], rfid::command::frame::set_tag_detection_mode[7],
			rfid::command::frame::set_tag_detection_mode[8], rfid::command::frame::set_tag_detection_mode[9],
			rfid::command::frame::set_tag_detection_mode[10], rfid::command::frame::set_tag_detection_mode[11],
			rfid::command::frame::set_tag_detection_mode[12], rfid::command::frame::set_tag_detection_mode[13],
			rfid::command::frame::set_tag_detection_mode[14], rfid::command::frame::set_tag_detection_mode[15]);

		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
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

TEST_F(CoreRFIDReaderTest, enableTagDetection)
{
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1);

	reader.init();
}

TEST_F(CoreRFIDReaderTest, registerCallbacks)
{
	reader.registerOnTagDetectedCallback(callbackFunction);
	reader.registerOnTagReadableCallback(callbackFunction);
}

TEST_F(CoreRFIDReaderTest, setTagDetectionMode)
{
	sendSetModeTagDetection();

	reader.setTagDetectionMode();
}

TEST_F(CoreRFIDReaderTest, onDataAvailableSuccess)
{
	setFakeCallback();

	std::array<uint8_t, 3> expected_tag_detection_callback = {0x00, 0x01, 0x02};

	reader.registerOnTagDetectedCallback(callbackFunction);

	receiveRFIDReaderAnswer(expected_tag_detection_callback);

	reader.onDataAvailable();

	auto is_tag_detected = reader.isTagDetected();

	EXPECT_TRUE(is_tag_detected);
}

TEST_F(CoreRFIDReaderTest, onDataAvailableWrongCallback)
{
	setFakeCallback();

	std::array<uint8_t, 3> expected_tag_detection_callback = {0x00, 0x01, 0xff};

	reader.registerOnTagDetectedCallback(callbackFunction);

	receiveRFIDReaderAnswer(expected_tag_detection_callback);

	reader.onDataAvailable();

	auto is_tag_detected = reader.isTagDetected();

	EXPECT_FALSE(is_tag_detected);
}

TEST_F(CoreRFIDReaderTest, onDataAvailableTooLongCallback)
{
	setFakeCallback();

	std::array<uint8_t, 4> expected_tag_detection_callback = {0x00, 0x01, 0x03, 0x04};

	reader.registerOnTagDetectedCallback(callbackFunction);

	receiveRFIDReaderAnswer(expected_tag_detection_callback);

	reader.onDataAvailable();

	auto is_tag_detected = reader.isTagDetected();

	EXPECT_FALSE(is_tag_detected);
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolSuccess)
{
	setFakeCallback();

	std::array<uint8_t, 2> set_protocol_success_answer			  = {0x00, 0x00};
	std::array<uint8_t, 2> set_gain_and_modulation_success_answer = {0x00, 0x00};

	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_success_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	reader.onDataAvailable();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnAnswerTooBig)
{
	setFakeCallback();
	std::array<uint8_t, 3> set_protocol_failed_answer = {0x00, 0x00, 0x00};
	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_failed_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	reader.onDataAvailable();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, setCommunicationProtocolFailedOnWrongFirstValue)
{
	setFakeCallback();
	std::array<uint8_t, 2> set_protocol_failed_answer = {0x82, 0x00};
	{
		InSequence seq;
		receiveRFIDReaderAnswer(set_protocol_failed_answer);
		sendSetProtocol();
		sendSetGainAndModulation();
	}

	reader.onDataAvailable();
	reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
}

TEST_F(CoreRFIDReaderTest, sendCommandSuccess)
{
	std::array<uint8_t, 2> command = {0x26, 0x07};
	const auto expected_values	   = ElementsAre(rfid::command::send_receive, command.size(), command[0], command[1]);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	reader.sendToTag(command);
}

TEST_F(CoreRFIDReaderTest, receiveDataSuccess)
{
	setFakeCallback();

	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C};

	std::array<uint8_t, 18> actual_values {0};

	receiveRFIDReaderAnswer(read_values);

	reader.onDataAvailable();
	reader.getDataFromTag(actual_values);

	bool is_communication_succeed = reader.didTagCommunicationSucceed(actual_values.size());

	EXPECT_EQ(is_communication_succeed, true);
	EXPECT_EQ(actual_values, expected_values);
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongAnswerFlag)
{
	setFakeCallback();

	std::array<uint8_t, 7> read_values = {0xff, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> actual_values {0};

	receiveRFIDReaderAnswer(read_values);

	reader.onDataAvailable();

	reader.getDataFromTag(actual_values);
	bool is_communication_succeed = reader.didTagCommunicationSucceed(read_values.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_NE(actual_values, read_values);
}

TEST_F(CoreRFIDReaderTest, receiveDataFailedWrongLength)
{
	setFakeCallback();

	std::array<uint8_t, 7> read_values = {0x80, 0x02, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> actual_values {0};

	receiveRFIDReaderAnswer(read_values);

	reader.onDataAvailable();

	reader.getDataFromTag(actual_values);

	bool is_communication_succeed = reader.didTagCommunicationSucceed(read_values.size());

	EXPECT_EQ(is_communication_succeed, false);
	EXPECT_NE(actual_values, read_values);
}

TEST_F(CoreRFIDReaderTest, getTag)
{
	setFakeCallback();
	reader.registerOnTagReadableCallback(callbackFunction);

	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0x6C, 0x28, 0x00, 0x00};

	std::array<uint8_t, 16> expected_values = {0x4C, 0x45, 0x4B, 0x41, 0x00, 0x01, 0x01, 0x00,
											   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	std::array<uint8_t, 18> actual_values {0};

	receiveRFIDReaderAnswer(read_values);

	reader.onDataAvailable();
	reader.getDataFromTag(actual_values);

	reader.onTagDataReceived();

	auto tag = reader.getTag();

	EXPECT_EQ(tag.data, expected_values);
}
