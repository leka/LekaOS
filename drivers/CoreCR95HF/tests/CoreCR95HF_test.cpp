// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"
#include <cstdint>

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

	// void SetUp() override {}
	// void TearDown() override {}

	CoreCR95HF corecr95hf;
	CoreBufferedSerialMock mockBufferedSerial;

	void sendSetModeTagDetection()
	{
		const auto expected_values = ElementsAre(rfid::cr95hf::settings::idle_tag_detection::tag_detection_command,
												 rfid::cr95hf::settings::idle_tag_detection::length,
												 rfid::cr95hf::settings::idle_tag_detection::wu_source,
												 rfid::cr95hf::settings::idle_tag_detection::enter_control[0],
												 rfid::cr95hf::settings::idle_tag_detection::enter_control[1],
												 rfid::cr95hf::settings::idle_tag_detection::wu_control[0],
												 rfid::cr95hf::settings::idle_tag_detection::wu_control[1],
												 rfid::cr95hf::settings::idle_tag_detection::leave_control[0],
												 rfid::cr95hf::settings::idle_tag_detection::leave_control[1],
												 rfid::cr95hf::settings::idle_tag_detection::wu_periode,
												 rfid::cr95hf::settings::idle_tag_detection::oscillator_start,
												 rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_start,
												 rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[0],
												 rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[1],
												 rfid::cr95hf::settings::idle_tag_detection::swing_count,
												 rfid::cr95hf::settings::idle_tag_detection::max_sleep);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void sendAskIdn()
	{
		const auto expected_values = ElementsAre(rfid::cr95hf::command::idn::id, rfid::cr95hf::command::idn::length);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void sendSetBaudrate(uint8_t baudrate)
	{
		const auto expected_values =
			ElementsAre(rfid::cr95hf::command::set_baudrate::id, rfid::cr95hf::command::set_baudrate::length, baudrate);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
	}

	void sendSetProtocol()
	{
		const auto expected_values =
			ElementsAre(rfid::cr95hf::command::set_protocol::id, rfid::cr95hf::command::set_protocol::length,
						rfid::cr95hf::protocol::iso14443A.id,
						rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));
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

	template <size_t size>
	void receiveCR95HFAnswer(const std::array<uint8_t, size> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + size), Return(size)));
	}

	void receiveSetBaudrate(uint8_t returned_value, int answer_size)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(returned_value), Return(answer_size)));
	}
};

TEST_F(CoreCR95HFSensorTest, initialization)
{
	ASSERT_NE(&corecr95hf, nullptr);
}

TEST_F(CoreCR95HFSensorTest, enableTagDetection)
{
	EXPECT_CALL(mockBufferedSerial, sigio).Times(1);

	corecr95hf.init();
}

void callback() {};
TEST_F(CoreCR95HFSensorTest, registerTagAvailableCallback)
{
	corecr95hf.registerTagAvailableCallback(callback);
	auto ptr_to_callback = corecr95hf.getTagAvailableCallback();

	ASSERT_EQ(ptr_to_callback, callback);
}

TEST_F(CoreCR95HFSensorTest, onTagAvailableSuccess)
{
	std::array<uint8_t, 3> expected_tag_detection_callback = {0x00, 0x01, 0x02};

	corecr95hf.registerTagAvailableCallback(callback);
	{
		InSequence seq;
		receiveCR95HFAnswer(expected_tag_detection_callback);
		sendSetModeTagDetection();
	}

	corecr95hf.onTagAvailable();
}
TEST_F(CoreCR95HFSensorTest, onTagAvailableWrongCallback)
{
	std::array<uint8_t, 3> expected_tag_detection_callback = {0x00, 0x01, 0xff};

	corecr95hf.registerTagAvailableCallback(callback);

	receiveCR95HFAnswer(expected_tag_detection_callback);

	corecr95hf.onTagAvailable();
}

TEST_F(CoreCR95HFSensorTest, getIDNSuccess)
{
	std::array<uint8_t, 17> expected_idn = {0x00, 0x0F, 0x4E, 0x46, 0x43, 0x20, 0x46, 0x53, 0x32,
											0x4A, 0x41, 0x53, 0x54, 0x34, 0x00, 0x2A, 0xCE};

	{
		InSequence seq;
		sendAskIdn();
		receiveCR95HFAnswer(expected_idn);
	}

	auto idn = corecr95hf.getIDN();
	ASSERT_EQ(idn, expected_idn);
}

TEST_F(CoreCR95HFSensorTest, getIDNFailedOnSize)
{
	std::array<uint8_t, 15> wrong_idn = {0x00, 0x0F, 0x4E, 0x46, 0x43, 0x20, 0x46, 0x53,
										 0x32, 0x4A, 0x41, 0x53, 0x54, 0x34, 0x00};

	std::array<uint8_t, 17> expected_idn = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	{
		InSequence seq;
		sendAskIdn();
		receiveCR95HFAnswer(wrong_idn);
	}

	auto idn = corecr95hf.getIDN();
	ASSERT_EQ(idn, expected_idn);
}

TEST_F(CoreCR95HFSensorTest, getIDNFailedOnValues)
{
	std::array<uint8_t, 17> expected_idn = {0xFF, 0xFF, 0x4E, 0x46, 0x43, 0x20, 0x46, 0x53, 0x32,
											0x4A, 0x41, 0x53, 0x54, 0x34, 0x00, 0x2A, 0xCE};

	{
		InSequence seq;
		sendAskIdn();
		receiveCR95HFAnswer(expected_idn);
	}

	auto idn = corecr95hf.getIDN();
	ASSERT_NE(idn, expected_idn);
}

TEST_F(CoreCR95HFSensorTest, setBaudrateSuccess)
{
	uint8_t expected_baudrate = 0x55;

	{
		InSequence seq;
		sendSetBaudrate(expected_baudrate);
		receiveSetBaudrate(expected_baudrate, 1);
	}

	auto baudrate = corecr95hf.setBaudrate(expected_baudrate);
	ASSERT_EQ(baudrate, true);
}

TEST_F(CoreCR95HFSensorTest, setBaudrateFailedOnSize)
{
	uint8_t expected_baudrate = 0x55;

	{
		InSequence seq;
		sendSetBaudrate(expected_baudrate);
		receiveSetBaudrate(expected_baudrate, 2);
	}

	auto baudrate = corecr95hf.setBaudrate(expected_baudrate);
	ASSERT_EQ(baudrate, false);
}

TEST_F(CoreCR95HFSensorTest, setBaudrateFailedOnValue)
{
	uint8_t expected_baudrate = 0x55;
	uint8_t wrong_baudrate	  = 0xff;

	{
		InSequence seq;
		sendSetBaudrate(expected_baudrate);
		receiveSetBaudrate(wrong_baudrate, 1);
	}

	auto baudrate = corecr95hf.setBaudrate(expected_baudrate);
	ASSERT_EQ(baudrate, false);
}

TEST_F(CoreCR95HFSensorTest, setCommunicationProtocolSuccess)
{
	std::array<uint8_t, 2> set_protocol_success_answer			  = {0x00, 0x00};
	std::array<uint8_t, 2> set_gain_and_modulation_success_answer = {0x00, 0x00};

	{
		InSequence seq;

		sendSetProtocol();
		receiveCR95HFAnswer(set_protocol_success_answer);
		sendSetGainAndModulation();
		receiveCR95HFAnswer(set_gain_and_modulation_success_answer);
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
		receiveCR95HFAnswer(set_protocol_failed_answer);
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
		receiveCR95HFAnswer(set_protocol_failed_answer);
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
