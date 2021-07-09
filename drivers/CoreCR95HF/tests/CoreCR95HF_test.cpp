// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreBufferedSerial.h"

using namespace leka;
using namespace interface;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

class CoreCR95HFSensorTest : public ::testing::Test
{
  protected:
	CoreCR95HFSensorTest() : corecr95hf(mockBufferedSerial) {};

	// void setCommunicationProtocol() override {}
	// void TearDown() override {}

	CoreCR95HF corecr95hf;
	CoreBufferedSerialMock mockBufferedSerial;

	void sendSetProtocol()
	{
		const auto expected_values =
			ElementsAre(rfid::cr95hf::command::set_protocol::id, rfid::cr95hf::command::set_protocol::length,
						rfid::cr95hf::protocol::iso14443A.id,
						rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
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
		const auto expected_values = ElementsAre(rfid::cr95hf::command::set_gain_and_modulation::id,
												 rfid::cr95hf::command::set_gain_and_modulation::length,
												 rfid::cr95hf::settings::arc_b, rfid::cr95hf::settings::flag_increment,
												 rfid::cr95hf::settings::acr_b_index_for_gain_and_modulation,
												 rfid::cr95hf::protocol::iso14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void receiveSetGainAndModulationAnswer(const std::array<uint8_t, 2> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + 2), Return(2)));
	}
};

TEST_F(CoreCR95HFSensorTest, initialization)
{
	ASSERT_NE(&corecr95hf, nullptr);
}

TEST_F(CoreCR95HFSensorTest, setCommunicationProtocolSuccess)
{
	std::array<uint8_t, 2> set_protocol_success_answer			  = {0x00, 0x00};
	std::array<uint8_t, 2> set_gain_and_modulation_success_answer = {0x00, 0x00};

	{
		InSequence seq;

		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_success_answer);
		sendSetGainAndModulation();
		receiveSetGainAndModulationAnswer(set_gain_and_modulation_success_answer);
	}

	auto is_initialized = corecr95hf.setCommunicationProtocol(rfid::Protocol::ISO14443A);
	ASSERT_EQ(is_initialized, true);
}

TEST_F(CoreCR95HFSensorTest, setCommunicationProtocolFailedOnAnswerTooBig)
{
	std::array<uint8_t, 3> set_protocol_failed_answer = {0x00, 0x00, 0x00};
	{
		InSequence seq;

		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_failed_answer);
	}

	auto is_initialized = corecr95hf.setCommunicationProtocol(rfid::Protocol::ISO14443A);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreCR95HFSensorTest, setCommunicationProtocolFailedOnWrongFirstValue)
{
	std::array<uint8_t, 2> set_protocol_failed_answer = {0x82, 0x00};
	{
		InSequence seq;

		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_failed_answer);
	}

	auto is_initialized = corecr95hf.setCommunicationProtocol(rfid::Protocol::ISO14443A);
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreCR95HFSensorTest, sendCommandSuccess)
{
	std::array<uint8_t, 2> command = {0x26, 0x07};
	const auto expected_values =
		ElementsAre(rfid::cr95hf::command::send_receive, command.size(), command[0], command[1]);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corecr95hf.sendCommandToTag(command);
}

TEST_F(CoreCR95HFSensorTest, receiveDataSuccess)
{
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
										   0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0xDA, 0x48, 0x28, 0x00, 0x00};

	std::array<uint8_t, 18> expected_values = {0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01,
											   0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0xDA, 0x48};

	std::array<uint8_t, 18> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 23), Return(23)));

	uint8_t is_communication_succeed = corecr95hf.receiveDataFromTag(actual_values);

	ASSERT_EQ(is_communication_succeed, true);
	ASSERT_EQ(actual_values, expected_values);
}

TEST_F(CoreCR95HFSensorTest, receiveDataFailed)
{
	std::array<uint8_t, 6> read_values	   = {0x80, 0x04, 0xff, 0x28, 0x00, 0x00};
	std::array<uint8_t, 1> expected_values = {0};

	std::array<uint8_t, 1> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(false));

	uint8_t is_communication_succeed = corecr95hf.receiveDataFromTag(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
	ASSERT_EQ(actual_values, expected_values);
}

TEST_F(CoreCR95HFSensorTest, receiveDataFailedWrongAnswerFlag)
{
	std::array<uint8_t, 7> read_values = {0xff, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 7), Return(7)));

	uint8_t is_communication_succeed = corecr95hf.receiveDataFromTag(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
	ASSERT_NE(actual_values, read_values);
}

TEST_F(CoreCR95HFSensorTest, receiveDataFailedWrongLength)
{
	std::array<uint8_t, 7> read_values = {0x80, 0x02, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 7), Return(0)));

	uint8_t is_communication_succeed = corecr95hf.receiveDataFromTag(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
	ASSERT_NE(actual_values, read_values);
}
