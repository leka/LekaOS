// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "external/st_jpeg_utils.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreJPEGMode.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "mocks/leka/File.h"

using namespace leka;

using ::testing::_;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;

class CoreJPEGTest : public ::testing::Test
{
  protected:
	CoreJPEGTest() : corejpeg(halmock, modemock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	mock::CoreJPEGMode modemock;
	mock::File filemock;
	CoreJPEG corejpeg;
};

TEST_F(CoreJPEGTest, instantiation)
{
	ASSERT_NE(&corejpeg, nullptr);
}

TEST_F(CoreJPEGTest, handleConfigurationInstance)
{
	auto handle = corejpeg.getHandle();

	ASSERT_EQ(handle.Instance, JPEG);
}

TEST_F(CoreJPEGTest, handlePointerConfigurationInstance)
{
	auto handle = corejpeg.getHandlePointer();

	ASSERT_EQ(handle->Instance, JPEG);
}

TEST_F(CoreJPEGTest, getImageProperties)
{
	auto expected_properties = JPEGImageProperties {};

	expected_properties.ImageWidth	= 800;
	expected_properties.ImageHeight = 480;

	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(expected_properties), Return(HAL_OK)));

	auto actual_properties = corejpeg.getImageProperties();

	EXPECT_EQ(actual_properties.ImageWidth, expected_properties.ImageWidth);
	EXPECT_EQ(actual_properties.ImageHeight, expected_properties.ImageHeight);
	EXPECT_EQ(actual_properties.getWidthOffset(), expected_properties.getWidthOffset());
}

TEST_F(CoreJPEGTest, initializationSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_RCC_JPEG_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_Init).Times(1);
	}

	corejpeg.initialize();
}

TEST_F(CoreJPEGTest, decodeImage)
{
	EXPECT_CALL(modemock, decode).Times(1);

	corejpeg.decodeImage(&filemock);
}
