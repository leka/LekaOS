// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

#include "CoreBufferedSerial.h"
#include "gtest/gtest.h"
#include "mock_CoreBufferedSerial.h"
#include "stub_BufferedSerial.h"

using namespace leka;

class LKCoreRFIDSensorTest : public ::testing::Test
{
  protected:
	LKCoreRFIDSensorTest() : corerfid(mockBufferedSerial) {};

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreRFID corerfid;
	CoreBufferedSerialMock mockBufferedSerial;

	auto compareRfidTag(RFIDTag rfid_tag, RFIDTag &expected_values)
	{
		ASSERT_EQ(rfid_tag.UID, expected_values.UID);
		ASSERT_EQ(rfid_tag.crc_UID, expected_values.crc_UID);
		ASSERT_EQ(rfid_tag.SAK, expected_values.SAK);
		ASSERT_EQ(rfid_tag.data, expected_values.data);
	}
};

TEST_F(LKCoreRFIDSensorTest, initialization)
{
	ASSERT_NE(&corerfid, nullptr);
}

TEST_F(LKCoreRFIDSensorTest, setRFIDTag)
{
	RFIDTag expected_rfid_tag = {{0x88, 0x04, 0x61, 0xD5, 0x0, 0x0, 0x0, 0x0}, {0x38, 0x0}, {0x01, 0x02}, {0xff}};

	corerfid.setRFIDTag(expected_rfid_tag);

	compareRfidTag(corerfid.getRFIDTag(), expected_rfid_tag);
}
