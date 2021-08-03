// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTouch.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

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
