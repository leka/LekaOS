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

// TEST_F(CoreHTSTest, turnOnSequenceAndValue)
// {
// 	auto expected_values = ElementsAre(0xA0, 0x80);

// 	InSequence seq;
// 	{
// 		EXPECT_CALL(mocki2c, write(i2c_address, Pointee(HTS221_CTRL_REG1 | 0x80), _, _)).Times(1);
// 		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
// 		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
// 	}

// 	corehts.turnOn();
// }

TEST_F(LKCoreRFIDSensorTest, writeProtocol)
{
	const auto expected_values = ElementsAre(0x02, 0x02, 0x02, 0x00);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corerfid.writeProtocol();
}

TEST_F(LKCoreRFIDSensorTest, checkProtocolPass)
{
	uint8_t read_values[2] = {0x00, 0x00};
	// const uint8_t register_reset_value {0x00};
	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkProtocol(), true);
}

TEST_F(LKCoreRFIDSensorTest, checkProtocolfailed)
{
	uint8_t read_values[2] = {0x82, 0x00};
	// const uint8_t register_reset_value {0x00};
	EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<0>(*read_values), Return(0)));

	ASSERT_EQ(corerfid.checkProtocol(), false);
}
// TEST_F(LKCoreRFIDSensorTest, checkProtocolfailed)
// {
// 	uint8_t register_reset_value[2] = {0x00, 0x00};
// 	// const uint8_t register_reset_value {0x00};
// 	// EXPECT_CALL(mockBufferedSerial, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));

// 	ASSERT_EQ(corerfid.checkProtocol(), false);
// }
