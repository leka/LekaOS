// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTouch.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::ElementsAre;

class CoreDACTouchTest : public ::testing::Test

{
  protected:
	CoreDACTouchTest() : coreADCTouch(i2cMock, 0xC0) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreDACTouch coreADCTouch;
	LKCoreI2CMock i2cMock;
};

TEST_F(CoreDACTouchTest, DACInstantiation)
{
	ASSERT_NE(&coreADCTouch, nullptr);
}

TEST_F(CoreDACTouchTest, setVoltageReference)
{
	const auto data = ElementsAre(0x80);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.setVoltageReference(dac_touch::data::voltageReference::Vdd);
}

TEST_F(CoreDACTouchTest, setPowerMode)
{
	const auto data = ElementsAre(0xAC, 0x40);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.setPowerMode(dac_touch::data::powerMode::channelA::powerDown500K |
							  dac_touch::data::powerMode::channelC::powerDown1K);
}

TEST_F(CoreDACTouchTest, setGain)
{
	const auto data = ElementsAre(0xCF);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.setGain(dac_touch::data::gain2::all);
}

TEST_F(CoreDACTouchTest, writeToAllInputRegister)
{
	std::array<uint8_t, 2> value_to_write = {0x0f, 0xA8};

	const auto data = ElementsAre(0x0f, 0xA8);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.writeToAllInputRegister(value_to_write);
}

TEST_F(CoreDACTouchTest, writeToSpecificInputRegister)
{
	std::array<uint8_t, 2> value_to_write = {0x0f, 0xA8};

	const auto data = ElementsAre(0x46, 0x0f, 0xA8);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.writeToSpecificInputRegister(dac_touch::channel::D, value_to_write);
}

TEST_F(CoreDACTouchTest, writeToMemoryRegisterUntilChannelD)
{
	std::array<uint8_t, 2> value_to_write = {0xFF, 0xAA};

	const auto data = ElementsAre(0x50, 0xFF, 0xAA);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.writeToMemoryRegisterUntilChannelD(dac_touch::channel::A, value_to_write);
}

TEST_F(CoreDACTouchTest, writeToSpecificMemoryRegister)
{
	std::array<uint8_t, 2> value_to_write = {0xFF, 0xAA};

	const auto data = ElementsAre(0x62, 0xFF, 0xAA);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));

	coreADCTouch.writeToSpecificMemoryRegister(dac_touch::channel::B, value_to_write);
}
