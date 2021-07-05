// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDSI.hpp"

#include "gtest/gtest.h"
#include "internal/corevideo_config.h"
#include "mocks/leka/CoreVideo/CoreLTDC.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;

class CoreDSITest : public ::testing::Test
{
  protected:
	CoreDSITest() : coredsi(halmock, ltdcmock) {}

	LKCoreSTM32HalMock halmock;
	CoreLTDCMock ltdcmock;
	CoreDSI coredsi;
};

TEST_F(CoreDSITest, instantiation)
{
	ASSERT_NE(&coredsi, nullptr);
}
