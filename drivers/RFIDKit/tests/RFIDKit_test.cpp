// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

#include "CoreCR95HF.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreBufferedSerial.h"
#include "mocks/leka/CoreRFID.h"

using namespace leka;
using namespace interface;

using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArrayArgument;

class CoreCR95HFSensorTest : public ::testing::Test
{
  protected:
	CoreCR95HFSensorTest() : coreRfidReader(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	CoreCR95HF coreRfidReader;
	CoreBufferedSerialMock mockBufferedSerial;
};

class CoreRFIDKitTest : public CoreCR95HFSensorTest
{
  protected:
	CoreRFIDKitTest() : coreRfid(mockCR95HF) {};

	// void SetUp() override {}
	// void TearDown() override {}

	RFIDKit coreRfid;
	CoreRFIDMock mockCR95HF;
	CoreBufferedSerialMock mockBufferedSerial;

	std::array<uint8_t, 3> returned_values_callback	  = {0x00, 0x01, 0x02};
	std::array<uint8_t, 2> set_protocol_answer		  = {0x00, 0x00};
	std::array<uint8_t, 2> set_gain_modulation_answer = {0x00, 0x00};

	template <size_t size>
	void setup(const std::array<uint8_t, size> &returned_values_set_protocol,
			   const std::array<uint8_t, 2> &returned_values_set_gain_and_modulation)
	{
		receiveCallback(returned_values_callback);
		sendSetProtocol();
		receiveSetProtocolAnswer(returned_values_set_protocol);
		sendSetGainAndModulation();
		receiveSetGainAndModulationAnswer(returned_values_set_gain_and_modulation);
	}

	template <size_t size>
	void receiveCallback(const std::array<uint8_t, size> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + size), Return(size)));
	}

	void sendSetProtocol()
	{
		const auto expected_values_set_protocol =
			ElementsAre(rfid::cr95hf::command::set_protocol::id, rfid::cr95hf::command::set_protocol::length,
						rfid::cr95hf::protocol::iso14443A.id,
						rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_protocol));
	}

	template <size_t size>
	void receiveSetProtocolAnswer(const std::array<uint8_t, size> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + size), Return(size)));
	}

	void sendSetGainAndModulation()
	{
		const auto expected_values_set_gain_and_modulation = ElementsAre(
			rfid::cr95hf::command::set_gain_and_modulation::id, rfid::cr95hf::command::set_gain_and_modulation::length,
			rfid::cr95hf::settings::arc_b, rfid::cr95hf::settings::flag_increment,
			rfid::cr95hf::settings::acr_b_index_for_gain_and_modulation,
			rfid::cr95hf::protocol::iso14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_gain_and_modulation));
	}

	void receiveSetGainAndModulationAnswer(const std::array<uint8_t, 2> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + 2), Return(2)));
	}

	void writeREQARequest()
	{
		const auto expected_values = ElementsAre(0x04, 0x02, 0x26, 0x07);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void writeReadRequest()
	{
		const auto send_read_values = ElementsAre(0x04, 0x03, 0x30, 0x08, 0x28);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(send_read_values));
	}

	void receiveAQTA(const std::array<uint8_t, 7> &atqa_answer)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(1));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(atqa_answer), begin(atqa_answer) + atqa_answer.size()),
							Return(atqa_answer.size())));
	}

	void receiveTagData(const std::array<uint8_t, 23> &read_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(1));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + read_values.size()),
							Return(read_values.size())));
	}
};

TEST_F(CoreRFIDKitTest, initialization)
{
	ASSERT_NE(&coreRfid, nullptr);
}

void interruptFunction() {};
TEST_F(CoreRFIDKitTest, getSerialTEST)
{
	EXPECT_CALL(mockCR95HF, getSerial).WillOnce(ReturnRef(mockBufferedSerial));

	coreRfid.setInterrupt(interruptFunction);
}

// TEST_F(CoreRFIDKitTest, setInterrupt)
// {
// 	EXPECT_CALL(mockBufferedSerial, sigio);

// 	coreRfid.setInterrupt(interruptFunction);
// }

TEST_F(CoreRFIDKitTest, waitForTagDetectionTEST)
{
	EXPECT_CALL(mockCR95HF, enableTagDetection).Times(1);

	coreRfid.setReaderForTagDetection();
}

// TEST_F(CoreRFIDKitTest, waitForTagDetection)
// {
// 	const auto expected_values_init = ElementsAre(
// 		rfid::cr95hf::settings::idle_tag_detection::tag_detection_command,
// 		rfid::cr95hf::settings::idle_tag_detection::length, rfid::cr95hf::settings::idle_tag_detection::wu_source,
// 		rfid::cr95hf::settings::idle_tag_detection::enter_control[0],
// 		rfid::cr95hf::settings::idle_tag_detection::enter_control[1],
// 		rfid::cr95hf::settings::idle_tag_detection::wu_control[0],
// 		rfid::cr95hf::settings::idle_tag_detection::wu_control[1],
// 		rfid::cr95hf::settings::idle_tag_detection::leave_control[0],
// 		rfid::cr95hf::settings::idle_tag_detection::leave_control[1],
// 		rfid::cr95hf::settings::idle_tag_detection::wu_periode,
// 		rfid::cr95hf::settings::idle_tag_detection::oscillator_start,
// 		rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_start,
// 		rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[0],
// 		rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[1],
// 		rfid::cr95hf::settings::idle_tag_detection::swing_count, rfid::cr95hf::settings::idle_tag_detection::max_sleep);
// 	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_init));

// 	coreRfid.setReaderForTagDetection();
// }

TEST_F(CoreRFIDKitTest, getTagDataSuccessTEST)
{
	{
		InSequence seq;
		EXPECT_CALL(mockCR95HF, send).Times(2);
		EXPECT_CALL(mockCR95HF, receiveTagData).Times(2);
	}

	std::array<uint8_t, 16> expected_values = {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
											   0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04};

	std::array<uint8_t, 16> actual_values {0};

	bool is_communication_succeed = coreRfid.getTagData(actual_values);

	ASSERT_EQ(is_communication_succeed, true);
	ASSERT_EQ(actual_values, expected_values);
}

TEST_F(CoreRFIDKitTest, getTagDataSuccess)
{
	std::array<uint8_t, 7> atqa_answer {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
										   0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x24, 0xF5, 0x28, 0x00, 0x00};
	{
		InSequence seq;
		setup(set_protocol_answer, set_gain_modulation_answer);

		writeREQARequest();
		receiveAQTA(atqa_answer);

		writeReadRequest();
		receiveTagData(read_values);
	}

	std::array<uint8_t, 16> expected_values = {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
											   0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04};

	std::array<uint8_t, 16> actual_values {0};

	bool is_communication_succeed = coreRfid.getTagData(actual_values);

	ASSERT_EQ(is_communication_succeed, true);
	ASSERT_EQ(actual_values, expected_values);
}

TEST_F(CoreRFIDKitTest, setupFailedOnWrongCallbackValues)
{
	std::array<uint8_t, 3> callback_values = {0x00, 0x01, 0x01};
	std::array<uint8_t, 16> actual_values {0};

	{
		InSequence seq;
		receiveCallback(callback_values);
	}

	auto is_initialized = coreRfid.getTagData(actual_values);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, setupFailedOnWrongCallbackSize)
{
	std::array<uint8_t, 4> callback_values = {0x00, 0x01, 0x02, 0x00};
	std::array<uint8_t, 16> actual_values {0};

	{
		InSequence seq;
		receiveCallback(callback_values);
	}

	auto is_initialized = coreRfid.getTagData(actual_values);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, setupFailedOnSetProtocolAnswerTooSmall)
{
	std::array<uint8_t, 3> set_protocol_failed_answer = {0x82, 0x00, 0x00};
	std::array<uint8_t, 16> actual_values {0};

	{
		InSequence seq;
		receiveCallback(returned_values_callback);
		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_failed_answer);
	}

	auto is_initialized = coreRfid.getTagData(actual_values);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, setupFailedOnSetProtocolOnFirstValue)
{
	set_protocol_answer = {0x82, 0x00};

	std::array<uint8_t, 16> actual_values {0};

	{
		InSequence seq;

		receiveCallback(returned_values_callback);
		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_answer);
	}

	auto is_initialized = coreRfid.getTagData(actual_values);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, setupFailedOnSetGainAndModulation)
{
	set_gain_modulation_answer = {0x00, 0xff};

	std::array<uint8_t, 16> actual_values {0};

	{
		InSequence seq;

		setup(set_protocol_answer, set_gain_modulation_answer);
	}

	auto is_initialized = coreRfid.getTagData(actual_values);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, getTagDataFailedOnReceiveATQA)
{
	std::array<uint8_t, 7> atqa_answer {0x80, 0x05, 0xFF, 0x00, 0x28, 0x00, 0x00};

	{
		InSequence seq;
		setup(set_protocol_answer, set_gain_modulation_answer);
		writeREQARequest();
		receiveAQTA(atqa_answer);
	}
	std::array<uint8_t, 16> actual_values {0};
	bool is_communication_succeed = coreRfid.getTagData(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
}

TEST_F(CoreRFIDKitTest, getTagDataFailedOnReceiveTagData)
{
	std::array<uint8_t, 7> atqa_answer {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
										   0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0xFF, 0xFF, 0x28, 0x00, 0x00};
	{
		InSequence seq;

		setup(set_protocol_answer, set_gain_modulation_answer);

		writeREQARequest();
		receiveAQTA(atqa_answer);

		writeReadRequest();
		receiveTagData(read_values);
	}

	std::array<uint8_t, 16> actual_values {0};
	bool is_communication_succeed = coreRfid.getTagData(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
}
