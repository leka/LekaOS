// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

#include "CoreBufferedSerial.h"
#include "gtest/gtest.h"
#include "mock_CoreBufferedSerial.h"
#include "stub_BufferedSerial.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

using namespace leka;

class LKCoreRFIDSensorTest : public ::testing::Test
{
  protected:
	LKCoreRFIDSensorTest() : corerfid(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreRFID corerfid;
	LKCoreBufferedSerialMock mockBufferedSerial;

	auto compareRfidTag(RFIDTag &rfid_tag, RFIDTag &expected_values)
	{
		ASSERT_EQ(rfid_tag.UID[0], expected_values.UID[0]);
		ASSERT_EQ(rfid_tag.UID[1], expected_values.UID[1]);
		ASSERT_EQ(rfid_tag.UID[2], expected_values.UID[2]);
		ASSERT_EQ(rfid_tag.UID[3], expected_values.UID[3]);
		ASSERT_EQ(rfid_tag.UID[4], expected_values.UID[4]);
		ASSERT_EQ(rfid_tag.UID[5], expected_values.UID[5]);
		ASSERT_EQ(rfid_tag.UID[6], expected_values.UID[6]);

		ASSERT_EQ(rfid_tag.SAK[0], expected_values.SAK[0]);
		ASSERT_EQ(rfid_tag.SAK[1], expected_values.SAK[1]);
		ASSERT_EQ(rfid_tag.SAK[2], expected_values.SAK[2]);
		ASSERT_EQ(rfid_tag.SAK[3], expected_values.SAK[3]);
	}
};

TEST_F(LKCoreRFIDSensorTest, initialization)
{
	ASSERT_NE(&corerfid, nullptr);
}

TEST_F(LKCoreRFIDSensorTest, writeProtocol)
{
	const auto expected_values = ElementsAre(0x02, 0x02, 0x02, 0x00);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.writeProtocol();
}

TEST_F(LKCoreRFIDSensorTest, setGain)
{
	const auto expected_values = ElementsAre(0x09, 0x04, 0x68, 0x01, 0x01, 0xD1);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.setGain();
}

TEST_F(LKCoreRFIDSensorTest, checkSensorSetPass)
{
	uint8_t read_values[2] = {0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkSensorSet(), true);
}

TEST_F(LKCoreRFIDSensorTest, checkSensorSetfailedOnFirstValue)
{
	uint8_t read_values[2] = {0x82, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 2), Return(0)));

	ASSERT_EQ(corerfid.checkSensorSet(), false);
}

TEST_F(LKCoreRFIDSensorTest, checkSensorSetfailedOnSecondValue)
{
	uint8_t read_values[2] = {0x00, 0x82};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 2), Return(0)));

	ASSERT_EQ(corerfid.checkSensorSet(), false);
}

TEST_F(LKCoreRFIDSensorTest, sendREQA)
{
	const auto expected_values = ElementsAre(0x04, 0x02, 0x26, 0x07);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendREQA();
}

TEST_F(LKCoreRFIDSensorTest, checkATQAPass)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x04, 0x00, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.checkATQA(), true);
}

TEST_F(LKCoreRFIDSensorTest, checkATQAFailOnFirstId)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.checkATQA(), false);
}

TEST_F(LKCoreRFIDSensorTest, checkATQAFailOnLastId)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x04, 0x44, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.checkATQA(), false);
}

TEST_F(LKCoreRFIDSensorTest, sendCL1)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x93, 0x20, 0x08);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendCL1();
}

TEST_F(LKCoreRFIDSensorTest, receiveUID1)
{
	uint8_t read_values[10] = {0x80, 0x08, 0x88, 0x04, 0x17, 0x9F, 0x04, 0x28, 0x00, 0x00};
	RFIDTag expected_values = {{0x88, 0x04, 0x17, 0x9F}, {0}, 0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 10), Return(0)));

	corerfid.receiveUID1();

	compareRfidTag(corerfid._rfid_tag, expected_values);
}

TEST_F(LKCoreRFIDSensorTest, sendUID1)
{
	const auto expected_values = ElementsAre(0x04, 0x08, 0x93, 0x70, 0x88, 0x04, 0x61, 0xD5, 0x38, 0x28);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendUID1();
}
