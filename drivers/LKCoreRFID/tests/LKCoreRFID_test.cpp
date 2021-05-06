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

	auto compareRfidTag(RFIDTag &rfid_tag, uint8_t *expected_id)
	{
		ASSERT_EQ(rfid_tag.result_code, 0x80);
		ASSERT_EQ(rfid_tag.length, 0x08);

		ASSERT_EQ(rfid_tag.id[0], expected_id[0]);
		ASSERT_EQ(rfid_tag.id[1], expected_id[1]);
		ASSERT_EQ(rfid_tag.id[2], expected_id[2]);
		ASSERT_EQ(rfid_tag.id[3], expected_id[3]);

		ASSERT_EQ(rfid_tag.check_sum, 0x04);
		ASSERT_EQ(rfid_tag.checks, 0x28);
		ASSERT_EQ(rfid_tag.collisionbyte, 0x00);
		ASSERT_EQ(rfid_tag.collisionbit, 0x00);
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

TEST_F(LKCoreRFIDSensorTest, checkSensorSetfailed)
{
	uint8_t read_values[2] = {0x82, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkSensorSet(), false);
}

TEST_F(LKCoreRFIDSensorTest, RFIDMessageToStruct)
{
	uint8_t tag_message[10] = {0x80, 0x08, 0x88, 0x04, 0x17, 0x9F, 0x04, 0x28, 0x00, 0x00};
	uint8_t expected_id[4]	= {0x88, 0x04, 0x17, 0x9F};
	RFIDTag rfid_tag		= {0, 0, 0, 0, 0, 0, 0};

	corerfid.RFIDMessageIntoStruct(tag_message, rfid_tag);

	compareRfidTag(rfid_tag, expected_id);
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

TEST_F(LKCoreRFIDSensorTest, checkATQAFail)
{
	uint8_t read_values[7] = {0x80, 0x05, 0x44, 0x00, 0x28, 0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 7), Return(0)));

	ASSERT_EQ(corerfid.checkATQA(), false);
}

TEST_F(LKCoreRFIDSensorTest, sendCL1)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x93, 0x20, 0x08);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.sendCL1();
}

TEST_F(LKCoreRFIDSensorTest, receiveID)
{
	uint8_t read_values[10] = {0x80, 0x08, 0x88, 0x04, 0x17, 0x9F, 0x04, 0x28, 0x00, 0x00};
	uint8_t expected_id[4]	= {0x88, 0x04, 0x17, 0x9F};
	RFIDTag rfid_tag		= {0, 0, 0, 0, 0, 0, 0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(read_values, read_values + 10), Return(0)));

	rfid_tag = corerfid.receiveID();

	compareRfidTag(rfid_tag, expected_id);
}
