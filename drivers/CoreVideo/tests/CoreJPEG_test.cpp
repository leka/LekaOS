// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreVideo/CoreJPEGMode.h"
#include "mocks/leka/LKCoreSTM32Hal.h"
#include "st_jpeg_utils.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;

class CoreJPEGTest : public ::testing::Test
{
  protected:
	CoreJPEGTest() : corejpeg(halmock, corejpegmodemock) {}

	LKCoreSTM32HalMock halmock;
	CoreJPEGModeMock corejpegmodemock;

	CoreJPEG corejpeg;
};


TEST_F(CoreJPEGTest, instantiation)
{
	ASSERT_NE(&corejpeg, nullptr);
}
