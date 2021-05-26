// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

#include "gtest/gtest.h"
#include "mock_CoreBufferedSerial.h"
#include "stub_BufferedSerial.h"

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::SetArrayArgument;

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

TEST_F(CoreCR95HFSensorTest, receiveAQTA)
{
	std::array<uint8_t, 7> read_values = {0x80, 0x05, 0x44, 0x0, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> expected_values {0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 7), Return(0)));

	corecr95hf.receive(expected_values.data(), expected_values.size());
	ASSERT_EQ(read_values, expected_values);
}

TEST_F(CoreCR95HFSensorTest, receiveRead)
{
	std::array<uint8_t, 20> read_values = {0x80, 0x15, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
										   0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0xDA, 0x48};
	std::array<uint8_t, 20> expected_values {0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 20), Return(0)));

	corecr95hf.receive(expected_values.data(), expected_values.size());
	ASSERT_EQ(read_values, expected_values);
}

TEST_F(CoreCR95HFSensorTest, receiveFailed)
{
	std::array<uint8_t, 7> read_values = {0x80, 0x05, 0x44, 0x0, 0x28, 0x00, 0x00};
	std::array<uint8_t, 7> expected_values {0};

	EXPECT_CALL(mockBufferedSerial, read)
		.WillOnce(DoAll(SetArrayArgument<0>(begin(read_values), begin(read_values) + 2), Return(0)));

	corecr95hf.receive(expected_values.data(), expected_values.size());
	ASSERT_NE(read_values, expected_values);
}
