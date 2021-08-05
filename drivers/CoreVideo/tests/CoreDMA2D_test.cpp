// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDMA2D.h"

#include "corevideo_config.h"
#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class CoreDMA2DTest : public ::testing::Test
{
  protected:
	CoreDMA2DTest() : dma2d(hal) {}

	LKCoreSTM32HalMock hal;
	CoreDMA2D dma2d;
};

TEST_F(CoreDMA2DTest, instantiation)
{
	ASSERT_NE(&dma2d, nullptr);
}
