// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreBufferedSerial.h"

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

using namespace leka;
using namespace interface;

class CoreCR95HFSensorTest : public ::testing::Test
{
  protected:
	CoreCR95HFSensorTest() : corecr95hf(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	CoreCR95HF corecr95hf;
	CoreBufferedSerialMock mockBufferedSerial;
};

TEST_F(CoreCR95HFSensorTest, initialization)
{
	ASSERT_NE(&corecr95hf, nullptr);
}

TEST_F(CoreCR95HFSensorTest, sendCommandSuccess)
{
	std::array<uint8_t, 2> command = {0x26, 0x07};
	const auto expected_values	   = ElementsAre(cr95hf::command::send_receive, command.size(), command[0], command[1]);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corecr95hf.send(command);
}

TEST_F(CoreCR95HFSensorTest, receiveDataSuccess)
{
	std::array<uint8_t, 20> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
										   0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0xDA, 0x48};
	std::array<uint8_t, 20> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 20), Return(20)));

	ASSERT_EQ(corecr95hf.receive(actual_values), 20);
	ASSERT_EQ(read_values, actual_values);
}

TEST_F(CoreCR95HFSensorTest, receiveDataFailed)
{
	std::array<uint8_t, 7> read_values = {0x80, 0x05, 0x44, 0x0, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> actual_values {0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 2), Return(-1)));

	ASSERT_EQ(corecr95hf.receive(actual_values), 0);
	ASSERT_NE(read_values, actual_values);
}

TEST_F(CoreCR95HFSensorTest, initSuccess)
{
	{
		InSequence seq;

		const auto expected_values_set_protocol =
			ElementsAre(cr95hf::command::set_protocol, 0x02, cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_protocol));

		std::array<uint8_t, 2> receive_values = {0x00, 0x00};
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(receive_values), begin(receive_values) + 2), Return(0)));

		const auto expected_values_set_gain_and_modulation =
			ElementsAre(cr95hf::command::set_gain_and_modulation, 0x04, cr95hf::arc_b, cr95hf::flag_increment,
						cr95hf::gain_modulation_index, cr95hf::protocol::ISO14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_gain_and_modulation));

		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(receive_values), begin(receive_values) + 2), Return(0)));
	}

	ASSERT_EQ(corecr95hf.init(), true);
}

TEST_F(CoreCR95HFSensorTest, initFailedOnSetProtocolOnFirstValue)
{
	{
		InSequence seq;

		const auto expected_values_set_protocol =
			ElementsAre(cr95hf::command::set_protocol, 0x02, cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_protocol));

		std::array<uint8_t, 2> receive_values_protocol = {0x04, 0x00};
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(receive_values_protocol), begin(receive_values_protocol) + 2),
							Return(0)));
	}

	ASSERT_EQ(corecr95hf.init(), false);
}

TEST_F(CoreCR95HFSensorTest, initFailedOnSetGainAndModulation)
{
	{
		InSequence seq;

		const auto expected_values_set_protocol =
			ElementsAre(cr95hf::command::set_protocol, 0x02, cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag);
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_protocol));

		std::array<uint8_t, 2> receive_values_set_protocol = {0x00, 0x00};
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(
				DoAll(SetArrayArgument<0>(begin(receive_values_set_protocol), begin(receive_values_set_protocol) + 2),
					  Return(0)));

		const auto expected_values_set_gain_and_modulation =
			ElementsAre(cr95hf::command::set_gain_and_modulation, 0x04, cr95hf::arc_b, cr95hf::flag_increment,
						cr95hf::gain_modulation_index, cr95hf::protocol::ISO14443A.gain_modulation_values());
		EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values_set_gain_and_modulation));

		std::array<uint8_t, 2> receive_values_set_gain_and_modulation = {0x00, 0xff};
		EXPECT_CALL(mockBufferedSerial, read)
			.WillOnce(DoAll(SetArrayArgument<0>(begin(receive_values_set_gain_and_modulation),
												begin(receive_values_set_gain_and_modulation) + 2),
							Return(0)));
	}

	ASSERT_EQ(corecr95hf.init(), false);
}
