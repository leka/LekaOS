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
