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

using namespace leka;

class LKCoreRFIDSensorTest : public ::testing::Test
{
  protected:
	LKCoreRFIDSensorTest() : corerfid(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreRFID corerfid;
	LKCoreBufferedSerialMock mockBufferedSerial;
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

TEST_F(LKCoreRFIDSensorTest, checkProtocolPass)
{
	uint8_t read_values[2] = {0x00, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkProtocol(), true);
}

TEST_F(LKCoreRFIDSensorTest, checkProtocolfailed)
{
	uint8_t read_values[2] = {0x82, 0x00};

	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkProtocol(), false);
}

TEST_F(LKCoreRFIDSensorTest, RFIDMessageToStruct)
{
	uint8_t tag_message[10] = {0x80, 0x08, 0x79, 0xC6, 0x7B, 0xBF, 0x7B, 0x28, 0x00, 0x00};
	uint8_t expected_id[5]	= {0x79, 0xC6, 0x7B, 0xBF, 0x7B};
	RFIDTag rfid_tag		= {0, 0, 0, 0, 0, 0};

	ASSERT_EQ(corerfid.RFIDMessageIntoStruct(tag_message, rfid_tag), true);

	ASSERT_EQ(rfid_tag.result_code, 0x80);
	ASSERT_EQ(rfid_tag.length, 0x08);

	ASSERT_EQ(rfid_tag.id[0], expected_id[0]);
	ASSERT_EQ(rfid_tag.id[1], expected_id[1]);
	ASSERT_EQ(rfid_tag.id[2], expected_id[2]);
	ASSERT_EQ(rfid_tag.id[3], expected_id[3]);
	ASSERT_EQ(rfid_tag.id[4], expected_id[4]);

	ASSERT_EQ(rfid_tag.checks, 0x28);
	ASSERT_EQ(rfid_tag.collisionbyte, 0x00);
	ASSERT_EQ(rfid_tag.collisionbit, 0x00);
}

TEST_F(LKCoreRFIDSensorTest, RFIDMessageToStructWrongResultCode)
{
	uint8_t tag_message[2] = {0x05};

	RFIDTag rfid_tag = {0, 0, 0, 0, 0, 0};
	ASSERT_EQ(corerfid.RFIDMessageIntoStruct(tag_message, rfid_tag), false);

	ASSERT_EQ(rfid_tag.result_code, 0x00);
}

TEST_F(LKCoreRFIDSensorTest, RFIDMessageToStructWrongLength)
{
	uint8_t tag_message[2] = {0x80, 0x4F};

	RFIDTag rfid_tag = {0, 0, 0, 0, 0, 0};
	ASSERT_EQ(corerfid.RFIDMessageIntoStruct(tag_message, rfid_tag), false);

	ASSERT_EQ(rfid_tag.length, 0x00);
}
