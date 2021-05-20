// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFID.h"

#include "CoreBufferedSerial.h"
#include "gtest/gtest.h"
#include "mock_CoreBufferedSerial.h"
#include "stub_BufferedSerial.h"

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::SetArrayArgument;

using namespace leka;

class CoreRFIDSensorTest : public ::testing::Test
{
  protected:
	CoreRFIDSensorTest() : corerfid(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	CoreRFID corerfid;
	CoreBufferedSerialMock mockBufferedSerial;

	auto compareRfidTag(RFIDTag rfid_tag, RFIDTag &expected_values)
	{
		ASSERT_EQ(rfid_tag.UID, expected_values.UID);
		ASSERT_EQ(rfid_tag.crc_UID, expected_values.crc_UID);
		ASSERT_EQ(rfid_tag.SAK, expected_values.SAK);
		ASSERT_EQ(rfid_tag.data, expected_values.data);
	}
};

TEST_F(CoreRFIDSensorTest, initialization)
{
	ASSERT_NE(&corerfid, nullptr);
}

TEST_F(CoreRFIDSensorTest, setRFIDTag)
{
	RFIDTag expected_rfid_tag = {{0x88, 0x04, 0x61, 0xD5, 0x0, 0x0, 0x0, 0x0}, {0x38, 0x0}, {0x01, 0x02}, {0xff}};

	corerfid.setRFIDTag(expected_rfid_tag);

	compareRfidTag(corerfid.getRFIDTag(), expected_rfid_tag);
}

TEST_F(CoreRFIDSensorTest, writeProtocol)
{
	const auto expected_values = ElementsAre(0x02, 0x02, 0x02, 0x00);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.setProtocol();
}

TEST_F(CoreRFIDSensorTest, setGain)
{
	const auto expected_values = ElementsAre(0x09, 0x04, 0x68, 0x01, 0x01, 0xD1);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.setGain();
}

TEST_F(CoreRFIDSensorTest, receiveSensorSetPass)
{
	uint8_t read_values[2] = {0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 2), Return(0)));

	ASSERT_EQ(corerfid.receiveSetupAnswer(), true);
}

TEST_F(CoreRFIDSensorTest, receiveSensorSetfailedOnFirstValue)
{
	uint8_t read_values[2] = {0x82, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 2), Return(0)));

	ASSERT_EQ(corerfid.receiveSetupAnswer(), false);
}

TEST_F(CoreRFIDSensorTest, receiveSensorSetfailedOnSecondValue)
{
	uint8_t read_values[2] = {0x00, 0x82};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 2), Return(0)));

	ASSERT_EQ(corerfid.receiveSetupAnswer(), false);
}

TEST_F(CoreRFIDSensorTest, sendREQA)
{
	const auto expected_values = ElementsAre(0x04, 0x02, 0x26, 0x07);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendREQA();
}

TEST_F(CoreRFIDSensorTest, checkATQAPass)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.receiveATQA(), true);
}

TEST_F(CoreRFIDSensorTest, checkATQAFailOnFirstId)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x04, 0x00, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.receiveATQA(), false);
}

TEST_F(CoreRFIDSensorTest, checkATQAFailOnLastId)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x44, 0x44, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.receiveATQA(), false);
}

TEST_F(CoreRFIDSensorTest, sendCL1)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x93, 0x20, 0x08);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendCL1();
}

TEST_F(CoreRFIDSensorTest, sendCL2)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x95, 0x20, 0x08);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendCL2();
}

TEST_F(CoreRFIDSensorTest, receiveUID1)
{
	uint8_t read_values[10] = {0x80, 0x08, 0x88, 0x04, 0x17, 0x9F, 0x04, 0x28, 0x00, 0x00};
	RFIDTag expected_values = {{0x88, 0x04, 0x17, 0x9F}, {0x04, 0}, {0}, 0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 10), Return(0)));

	corerfid.receiveUID1();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}

TEST_F(CoreRFIDSensorTest, receiveUID2)
{
	uint8_t read_values[10] = {0x80, 0x08, 0x32, 0x9B, 0x66, 0x80, 0x4F, 0x28, 0x00, 0x00};
	RFIDTag expected_values = {{0x00, 0x00, 0x00, 0x00, 0x32, 0x9B, 0x66, 0x80}, {0x00, 0x4F}, {0}, 0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 10), Return(0)));

	corerfid.receiveUID2();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}

TEST_F(CoreRFIDSensorTest, sendUID1)
{
	const auto expected_values = ElementsAre(0x04, 0x08, 0x93, 0x70, 0x88, 0x04, 0x61, 0xD5, 0x38, 0x28);

	RFIDTag expected_rfid_tag = {{0x88, 0x04, 0x61, 0xD5, 0x0, 0x0, 0x0, 0x0}, {0x38, 0x0}, {}, {}};

	corerfid.setRFIDTag(expected_rfid_tag);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendUID1();
}

TEST_F(CoreRFIDSensorTest, sendUID2)
{
	const auto expected_values = ElementsAre(0x04, 0x08, 0x95, 0x70, 0x32, 0x9B, 0x66, 0x80, 0x4F, 0x28);
	RFIDTag expected_rfid_tag  = {{0, 0, 0, 0, 0x32, 0x9B, 0x66, 0x80}, {0, 0x4F}, {}, {}};

	corerfid.setRFIDTag(expected_rfid_tag);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendUID2();
}

TEST_F(CoreRFIDSensorTest, receiveSAK1)
{
	uint8_t read_values[8]	= {0x80, 0x06, 0x04, 0xDA, 0x17, 0x08, 0x00, 0x00};
	RFIDTag expected_values = {{0}, {0}, {0x04, 0xDA}, 0};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 8), Return(0)));

	corerfid.receiveSAK1();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}

TEST_F(CoreRFIDSensorTest, receiveSAK2)
{
	uint8_t read_values[8]	= {0x80, 0x06, 0x00, 0xFE, 0x51, 0x08, 0x00, 0x00};
	RFIDTag expected_values = {{0}, {0}, {0x00, 0x00, 0x00, 0xFE}, 0};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 8), Return(0)));

	corerfid.receiveSAK2();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}

TEST_F(CoreRFIDSensorTest, authentification)
{
	const auto expected_values = ElementsAre(0x04, 0x06, 0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0x28);
	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.authentification();
}

TEST_F(CoreRFIDSensorTest, receiveAuthentificationSuccess)
{
	uint8_t read_values[9] = {0x0, 0x0, 0x0, 0x0, 0xA0, 0x1E, 0x0, 0x0, 0x0};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 9), Return(0)));

	ASSERT_EQ(corerfid.receiveAuthentification(), true);
}

TEST_F(CoreRFIDSensorTest, receiveAuthentificationFailedOnFirstValue)
{
	uint8_t read_values[9] = {0x0, 0x0, 0x0, 0x0, 0x88, 0x1E, 0x0, 0x0, 0x0};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 9), Return(0)));

	ASSERT_EQ(corerfid.receiveAuthentification(), false);
}

TEST_F(CoreRFIDSensorTest, receiveAuthentificationFailedOnSecondValue)
{
	uint8_t read_values[9] = {0x0, 0x0, 0x0, 0x0, 0xA0, 0x88, 0x0, 0x0, 0x0};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 9), Return(0)));

	ASSERT_EQ(corerfid.receiveAuthentification(), false);
}

TEST_F(CoreRFIDSensorTest, readRFIDTag)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x30, 0x05, 0x28);
	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.readRFIDTag();
}
TEST_F(CoreRFIDSensorTest, receiveRFIDTag)
{
	uint8_t read_values[20] = {
		0x80, 0x15, 0x34, 0x03, 0x00, 0xFE, 0x01, 0x02, 0x03, 0x04,
		0x0,  0x0,	0x0,  0x0,	0x0,  0x0,	0x0,  0x0,	0x5C, 0x4C};   // would have 0x08, 0x00, 0x00 more but
																	   // SetArrayArgument is limited to 20 values
	RFIDTag expected_values = {
		{0}, {0}, {0}, {0x34, 0x03, 0x00, 0xFE, 0x01, 0x02, 0x03, 0x04, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 20), Return(0)));

	corerfid.receiveRFIDTag();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}

TEST_F(CoreRFIDSensorTest, receiveRFIDTagWrongCRC)
{
	uint8_t read_values[20] = {
		0x80, 0x15, 0x34, 0x03, 0x00, 0xFE, 0x01, 0x02, 0x03, 0x04,
		0x0,  0x0,	0x0,  0x0,	0x0,  0x0,	0x0,  0x0,	0xAC, 0x4C};   // would have 0x08, 0x00, 0x00 more but
																	   // SetArrayArgument is limited to 20 values
	RFIDTag expected_values = {
		{0}, {0}, {0}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 20), Return(0)));

	corerfid.receiveRFIDTag();

	compareRfidTag(corerfid.getRFIDTag(), expected_values);
}
