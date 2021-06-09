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
using ::testing::ElementsAreArray;
using ::testing::Pointee;
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
};

class CoreRFIDKitTest : public CoreCR95HFSensorTest
{
  protected:
	CoreRFIDKitTest() : coreRfid(CoreCR95HFSensorTest::corecr95hf) {};

	// void SetUp() override {}
	// void TearDown() override {}

	RFIDKit coreRfid;
	// CoreRFIDMock mockRfid;
};

TEST_F(CoreRFIDKitTest, initialization)
{
	ASSERT_NE(&coreRfid, nullptr);
}

TEST_F(CoreRFIDKitTest, sendREQA)
{
	const auto expected_values = ElementsAre(0x04, 0x02, 0x26, 0x07);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	coreRfid.sendREQA();
}

TEST_F(CoreRFIDKitTest, sendReadRegister8)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x30, 0x08, 0x28);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	coreRfid.sendReadRegister8();
}
