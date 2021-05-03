// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

#include "gtest/gtest.h"

using namespace leka;

LKCoreRFID coreRFID;

TEST(LKCoreRFIDSensorTest, initialization)
{
	ASSERT_NE(&coreRFID, nullptr);
}

TEST(LKCoreRFIDSensorTest, RFIDMessageIntoStruct)
{
	uint8_t RFIDTag[6] = {0x01, 0x04, 0x01, 0x02, 0x03, 0x04};

	uint8_t expected_data[4]		= {0x01, 0x02, 0x03, 0x04};
	struct RFIDTag expected_RFIDtag = {0x01, 0x04};

	for (int i = 0; i < 4; i++) {
		expected_RFIDtag.data[i] = expected_data[i];
	}

	auto actual_RFIDtag = coreRFID.RFIDMessageIntoStruct(RFIDTag);

	ASSERT_EQ(actual_RFIDtag.command, expected_RFIDtag.command);
	ASSERT_EQ(actual_RFIDtag.length, expected_RFIDtag.length);
	ASSERT_EQ(actual_RFIDtag.data[0], expected_RFIDtag.data[0]);
}

TEST(LKCoreRFIDSensorTest, RFIDMessageIntoStructLengthToHigh)
{
	uint8_t RFIDTag[6] = {0x01, 0xFF, 0x01, 0x02, 0x03, 0x04};

	auto actual_RFIDtag = coreRFID.RFIDMessageIntoStruct(RFIDTag);

	ASSERT_EQ(actual_RFIDtag.command, 0);
	ASSERT_EQ(actual_RFIDtag.length, 0);
	ASSERT_EQ(actual_RFIDtag.data[0], 0);
}
