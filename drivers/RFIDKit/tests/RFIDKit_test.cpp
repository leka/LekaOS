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

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreCR95HFSensorTest : public ::testing::Test
{
  protected:
	CoreCR95HFSensorTest() : corecr95hf(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	CoreCR95HF corecr95hf;
	CoreBufferedSerialMock mockBufferedSerial;
};

class CoreRFIDKitTest : public CoreCR95HFSensorTest
{
  protected:
	CoreRFIDKitTest() : coreRfid(CoreCR95HFSensorTest::corecr95hf) {};

	// void SetUp() override {}
	// void TearDown() override {}

	RFIDKit coreRfid;

	void sendSetProtocol()
	{
		const auto expected_values_set_protocol =
			ElementsAre(rfid::cr95hf::command::set_protocol, 0x02, rfid::cr95hf::protocol::iso14443A.id,
						rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_protocol));
	}

	void receiveSetProtocolAnswer(const std::array<uint8_t, 2> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + 2), Return(0)));
	}

	void sendSetGainAndModulation()
	{
		const auto expected_values_set_gain_and_modulation = ElementsAre(
			rfid::cr95hf::command::set_gain_and_modulation, 0x04, rfid::cr95hf::settings::arc_b,
			rfid::cr95hf::settings::flag_increment, rfid::cr95hf::settings::acr_b_index_for_gain_and_modulation,
			rfid::cr95hf::protocol::iso14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_gain_and_modulation));
	}

	void receiveSetGainAndModulationAnswer(const std::array<uint8_t, 2> &returned_values)
	{
		EXPECT_CALL(mockBufferedSerial, readable).WillOnce(Return(true));
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(returned_values), begin(returned_values) + 2), Return(0)));
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

TEST_F(CoreRFIDKitTest, initSuccess)
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

	auto is_initialized = corecr95hf.init();
	ASSERT_EQ(is_initialized, true);
}

TEST_F(CoreRFIDKitTest, initFailed)
{
	std::array<uint8_t, 2> set_protocol_failed_answer = {0x82, 0x00};
	{
		InSequence seq;

		sendSetProtocol();
		receiveSetProtocolAnswer(set_protocol_failed_answer);
	}

	auto is_initialized = corecr95hf.init();
	ASSERT_EQ(is_initialized, false);
}

TEST_F(CoreRFIDKitTest, getTagDataSuccess)
{
	std::array<uint8_t, 7> atqa_answer {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};
	std::array<uint8_t, 23> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02,
										   0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x24, 0xF5, 0x28, 0x00, 0x00};
	{
		InSequence seq;

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

TEST_F(CoreRFIDKitTest, getTagDataFailedOnReceiveATQA)
{
	std::array<uint8_t, 7> atqa_answer {0x80, 0x05, 0xFF, 0x00, 0x28, 0x00, 0x00};

	{
		InSequence seq;

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

		writeREQARequest();
		receiveAQTA(atqa_answer);

		writeReadRequest();
		receiveTagData(read_values);
	}

	std::array<uint8_t, 16> actual_values {0};
	bool is_communication_succeed = coreRfid.getTagData(actual_values);

	ASSERT_EQ(is_communication_succeed, false);
}
