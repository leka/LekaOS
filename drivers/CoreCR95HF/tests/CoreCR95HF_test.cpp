// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

#include "gtest/gtest.h"
#include "mock_CoreBufferedSerial.h"
#include "stub_BufferedSerial.h"

using ::testing::Args;
using ::testing::ElementsAre;

using namespace leka;

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

TEST_F(CoreCR95HFSensorTest, sendSuccessREQA)
{
	const auto expected_values = ElementsAre(0x04, 0x02, 0x26, 0x07);
	auto REQACommand		   = static_cast<uint8_t>(0x26);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corecr95hf.send(&REQACommand, 1);
}

TEST_F(CoreCR95HFSensorTest, sendSuccessRead)
{
	const auto expected_values = ElementsAre(0x04, 0x03, 0x30, 0x08, 0x28);
	auto ReadCommand		   = std::array<uint8_t, 2> {0x30, 0x08};

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corecr95hf.send(ReadCommand.data(), 2);
}

TEST_F(CoreCR95HFSensorTest, sendWithoutArguments)
{
	const auto expected_values = ElementsAre(0x04, 0x01, 0x0);

	EXPECT_CALL(mockBufferedSerial, write).With(Args<0, 1>(expected_values));

	corecr95hf.send(nullptr, 0);
}
