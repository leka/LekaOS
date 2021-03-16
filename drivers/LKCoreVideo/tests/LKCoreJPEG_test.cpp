// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"

#include "gtest/gtest.h"
#include "mock_LKCoreDMA2D.h"
#include "mock_LKCoreFatFs.h"
#include "mock_LKCoreSTM32Hal.h"
#include "st_jpeg_utils.h"

using namespace leka;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;

class LKCoreJPEGTest : public ::testing::Test
{
  protected:
	LKCoreJPEGTest() : corejpeg(halmock, dma2dmock, filemock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreSTM32HalMock halmock;
	LKCoreDMA2DMock dma2dmock;
	LKCoreFatFsMock filemock;
	LKCoreJPEG corejpeg;
};

TEST_F(LKCoreJPEGTest, instantiation)
{
	ASSERT_NE(&corejpeg, nullptr);
}

TEST_F(LKCoreJPEGTest, handleConfigurationInstance)
{
	auto handle = corejpeg.getHandle();

	ASSERT_EQ(handle.Instance, JPEG);
}

TEST_F(LKCoreJPEGTest, handlePointerConfigurationInstance)
{
	auto handle = corejpeg.getHandlePointer();

	ASSERT_EQ(handle->Instance, JPEG);
}

TEST_F(LKCoreJPEGTest, getConfiguration)
{
	JPEG_ConfTypeDef expected_config;

	expected_config.ImageWidth	= 800;
	expected_config.ImageHeight = 480;

	// Setup
	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(expected_config), Return(HAL_OK)));

	// Apply setup
	FIL image;
	corejpeg.displayImage(&image);

	// Test
	auto config = corejpeg.getConfig();

	ASSERT_EQ(config.ImageWidth, expected_config.ImageWidth);
	ASSERT_EQ(config.ImageHeight, expected_config.ImageHeight);
}

TEST_F(LKCoreJPEGTest, initializationSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_RCC_JPEG_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_Init).Times(1);
	}

	corejpeg.initialize();
}

TEST_F(LKCoreJPEGTest, decodeImageWithPollingDecodeCalledSuccess)
{
	EXPECT_CALL(filemock, read).Times(1);
	EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(1);

	auto status = corejpeg.decodeImageWithPolling();

	ASSERT_EQ(status, HAL_OK);
}

TEST_F(LKCoreJPEGTest, decodeImageWithPollingDecodeCalledFailed)
{
	EXPECT_CALL(filemock, read).WillOnce(Return(FR_NO_FILE));

	auto status = corejpeg.decodeImageWithPolling();

	ASSERT_NE(status, HAL_OK);
}

TEST_F(LKCoreJPEGTest, getWidthOffsetNoChromaSubsampling)
{
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = 3;

	// Setup
	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(config), Return(HAL_OK)));

	// Apply setup
	FIL image;
	corejpeg.displayImage(&image);

	// Test
	auto width_offset = corejpeg.getWidthOffset();

	ASSERT_EQ(0, width_offset);
}

TEST_F(LKCoreJPEGTest, getWidthOffsetChromaSubsampling420)
{
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_420_SUBSAMPLING;

	for (int offset = 0; offset < 16; offset++) {
		// Setup
		config.ImageWidth = 16 * 50 + offset;	// =800 + i
		EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(config), Return(HAL_OK)));

		// Apply setup
		FIL image;
		corejpeg.displayImage(&image);

		// Test
		auto width_offset = corejpeg.getWidthOffset();

		ASSERT_EQ((16 - offset) % 16, width_offset);
	}
}

TEST_F(LKCoreJPEGTest, getWidthOffsetChromaSubsampling422)
{
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_422_SUBSAMPLING;

	for (int offset = 0; offset < 16; offset++) {
		// Setup
		config.ImageWidth = 16 * 50 + offset;	// =800 + i
		EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(config), Return(HAL_OK)));

		// Apply setup
		FIL image;
		corejpeg.displayImage(&image);

		// Test
		auto width_offset = corejpeg.getWidthOffset();

		ASSERT_EQ((16 - offset) % 16, width_offset);
	}
}

TEST_F(LKCoreJPEGTest, getWidthOffsetChromaSubsampling444)
{
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;

	for (int offset = 0; offset < 8; offset++) {
		// Setup
		config.ImageWidth = 8 * 100 + offset;	// =800 + i
		EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(config), Return(HAL_OK)));

		// Apply setup
		FIL image;
		corejpeg.displayImage(&image);

		// Test
		auto width_offset = corejpeg.getWidthOffset();

		ASSERT_EQ(offset % 8, width_offset);
	}
}

TEST_F(LKCoreJPEGTest, displaySequence)
{
	{
		InSequence seq;
		EXPECT_CALL(filemock, read).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_GetInfo).Times(1);
		EXPECT_CALL(dma2dmock, transferImage).Times(1);
	}

	FIL image;
	corejpeg.displayImage(&image);
}

TEST_F(LKCoreJPEGTest, onErroCallback)
{
	JPEG_HandleTypeDef hjpeg;

	corejpeg.onErrorCallback(&hjpeg);
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback420ChromSubsamplingDimensionsMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_420_SUBSAMPLING;
	config.ImageWidth		 = 16 * 50;	  // =800
	config.ImageHeight		 = 16 * 30;	  // =480

	corejpeg.onInfoReadyCallback(&hjpeg, &config);

	ASSERT_EQ(config.ImageWidth, 800);
	ASSERT_EQ(config.ImageHeight, 480);
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback420ChromSubsamplingDimensionsNotMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_420_SUBSAMPLING;

	for (int i = 1; i < 16; i++) {
		config.ImageWidth  = 16 * 50 + i;	// =800 + i
		config.ImageHeight = 16 * 30 + i;	// =480 + i

		corejpeg.onInfoReadyCallback(&hjpeg, &config);

		auto width_ceil_multiple  = 800 + 16 * ((15 + i) / 16);
		auto height_ceil_multiple = 480 + 16 * ((15 + i) / 16);

		ASSERT_EQ(config.ImageWidth, width_ceil_multiple);
		ASSERT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback422ChromSubsamplingDimensionsMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_422_SUBSAMPLING;
	config.ImageWidth		 = 16 * 50;	  // =800
	config.ImageHeight		 = 8 * 60;	  // =480

	corejpeg.onInfoReadyCallback(&hjpeg, &config);

	ASSERT_EQ(config.ImageWidth, 800);
	ASSERT_EQ(config.ImageHeight, 480);
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback422ChromSubsamplingDimensionsNotMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_422_SUBSAMPLING;

	for (int i = 1; i < 16; i++) {
		config.ImageWidth  = 16 * 50 + i;	// =800 + i
		config.ImageHeight = 8 * 60 + i;	// =480 + i

		corejpeg.onInfoReadyCallback(&hjpeg, &config);

		auto width_ceil_multiple  = 800 + 16 * ((15 + i) / 16);
		auto height_ceil_multiple = 480 + 8 * ((7 + i) / 8);

		ASSERT_EQ(config.ImageWidth, width_ceil_multiple);
		ASSERT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback444ChromSubsamplingDimensionsMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;
	config.ImageWidth		 = 8 * 100;	  // =800
	config.ImageHeight		 = 8 * 60;	  // =480

	corejpeg.onInfoReadyCallback(&hjpeg, &config);

	ASSERT_EQ(config.ImageWidth, 800);
	ASSERT_EQ(config.ImageHeight, 480);
}

TEST_F(LKCoreJPEGTest, onInfoReadyCallback444ChromSubsamplingDimensionsNotMultiple)
{
	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef config;

	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;

	for (int i = 1; i < 8; i++) {
		config.ImageWidth  = 8 * 100 + i;	// =800 + i
		config.ImageHeight = 8 * 60 + i;	// =480 + i

		corejpeg.onInfoReadyCallback(&hjpeg, &config);

		auto width_ceil_multiple  = 800 + 8 * ((7 + i) / 8);
		auto height_ceil_multiple = 480 + 8 * ((7 + i) / 8);

		ASSERT_EQ(config.ImageWidth, width_ceil_multiple);
		ASSERT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(LKCoreJPEGTest, onDataAvailableCallback)
{
	JPEG_HandleTypeDef hjpeg;
	uint32_t size;

	{
		InSequence seq;
		EXPECT_CALL(filemock, seek).Times(1);
		EXPECT_CALL(filemock, read).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(1);
	}

	corejpeg.onDataAvailableCallback(&hjpeg, size);
}

TEST_F(LKCoreJPEGTest, onDataReadyCallback)
{
	JPEG_HandleTypeDef hjpeg;
	uint8_t *pDataOut;
	uint32_t size = 0;
	JPEG_ConfTypeDef config;

	// Setup indrectictly pConvert_Function
	config.ColorSpace		 = JPEG_YCBCR_COLORSPACE;
	config.ChromaSubsampling = JPEG_420_SUBSAMPLING;
	corejpeg.onInfoReadyCallback(&hjpeg, &config);

	EXPECT_CALL(halmock, HAL_JPEG_ConfigOutputBuffer).Times(1);

	corejpeg.onDataReadyCallback(&hjpeg, pDataOut, size);
}

TEST_F(LKCoreJPEGTest, onDecodeCompleteCallback)
{
	JPEG_HandleTypeDef hjpeg;

	corejpeg.onDecodeCompleteCallback(&hjpeg);
}
