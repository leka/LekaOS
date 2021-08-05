// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLTDC.h"

#include "corevideo_config.h"
#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::Field;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Property;

class CoreLTDCTest : public ::testing::Test
{
  protected:
	CoreLTDCTest() : coreltdc(halmock) {}

	LKCoreSTM32HalMock halmock;

	CoreLTDC coreltdc;
};

TEST_F(CoreLTDCTest, instantiation)
{
	ASSERT_NE(&coreltdc, nullptr);
}
