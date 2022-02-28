// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "external/st_jpeg_utils.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreDMA2D.h"
#include "mocks/leka/CoreFatFs.h"
#include "mocks/leka/CoreSTM32Hal.h"

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
	CoreJPEGTest() : corejpeg(halmock, dma2dmock, filemock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreSTM32HalMock halmock;
	mock::CoreDMA2D dma2dmock;
	mock::CoreFatFs filemock;
	CoreJPEG corejpeg;

	// TODO: These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO: Remove them in the future
	void MOCK_FUNCTION_silenceUnexpectedCalls(void)
	{
		EXPECT_CALL(filemock, read).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(AnyNumber());
		EXPECT_CALL(dma2dmock, transferImage).Times(AnyNumber());
	}

	void TEST_FUNCTION_WidthOffsetFromChromaSubsampling(JPEG_ConfTypeDef config)
	{
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

TEST_F(CoreJPEGTest, getConfiguration)
{
	JPEG_ConfTypeDef expected_config;

	expected_config.ImageWidth	= 800;
	expected_config.ImageHeight = 480;

	// Setup
	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(expected_config), Return(HAL_OK)));

	MOCK_FUNCTION_silenceUnexpectedCalls();

	// Apply setup
	FIL image;
	corejpeg.displayImage(&image);

	// Test
	auto config = corejpeg.getConfig();

	ASSERT_EQ(config.ImageWidth, expected_config.ImageWidth);
	ASSERT_EQ(config.ImageHeight, expected_config.ImageHeight);
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

TEST_F(CoreJPEGTest, decodeImageWithPollingDecodeCalledSuccess)
{
	EXPECT_CALL(filemock, read).Times(1);
	EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(1);

	auto status = corejpeg.decodeImageWithPolling();

	ASSERT_EQ(status, HAL_OK);
}

TEST_F(CoreJPEGTest, decodeImageWithPollingDecodeCalledFailed)
{
	EXPECT_CALL(filemock, read).WillOnce(Return(FR_NO_FILE));

	auto status = corejpeg.decodeImageWithPolling();

	ASSERT_NE(status, HAL_OK);
}

TEST_F(CoreJPEGTest, getWidthOffsetNoChromaSubsampling)
{
	JPEG_ConfTypeDef config;
	config.ChromaSubsampling = 3;

	// Setup
	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(config), Return(HAL_OK)));
	MOCK_FUNCTION_silenceUnexpectedCalls();

	// Apply setup
	FIL image;
	corejpeg.displayImage(&image);

	// Test
	auto width_offset = corejpeg.getWidthOffset();

	ASSERT_EQ(0, width_offset);
}

TEST_F(CoreJPEGTest, getWidthOffsetChromaSubsampling420)
{
	JPEG_ConfTypeDef config;
	config.ChromaSubsampling = JPEG_420_SUBSAMPLING;

	MOCK_FUNCTION_silenceUnexpectedCalls();

	TEST_FUNCTION_WidthOffsetFromChromaSubsampling(config);
}

TEST_F(CoreJPEGTest, getWidthOffsetChromaSubsampling422)
{
	JPEG_ConfTypeDef config;
	config.ChromaSubsampling = JPEG_422_SUBSAMPLING;

	MOCK_FUNCTION_silenceUnexpectedCalls();

	TEST_FUNCTION_WidthOffsetFromChromaSubsampling(config);
}

TEST_F(CoreJPEGTest, getWidthOffsetChromaSubsampling444)
{
	JPEG_ConfTypeDef config;
	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;

	MOCK_FUNCTION_silenceUnexpectedCalls();

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

TEST_F(CoreJPEGTest, displaySequence)
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

TEST_F(CoreJPEGTest, onErroCallback)
{
	JPEG_HandleTypeDef hjpeg;

	corejpeg.onErrorCallback(&hjpeg);
}

TEST_F(CoreJPEGTest, onInfoReadyCallback420ChromSubsamplingDimensionsMultiple)
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

TEST_F(CoreJPEGTest, onInfoReadyCallback420ChromSubsamplingDimensionsNotMultiple)
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

TEST_F(CoreJPEGTest, onInfoReadyCallback422ChromSubsamplingDimensionsMultiple)
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

TEST_F(CoreJPEGTest, onInfoReadyCallback422ChromSubsamplingDimensionsNotMultiple)
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

TEST_F(CoreJPEGTest, onInfoReadyCallback444ChromSubsamplingDimensionsMultiple)
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

TEST_F(CoreJPEGTest, onInfoReadyCallback444ChromSubsamplingDimensionsNotMultiple)
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

TEST_F(CoreJPEGTest, onDataAvailableCallback)
{
	JPEG_HandleTypeDef hjpeg;
	uint32_t size {2};

	{
		InSequence seq;
		EXPECT_CALL(filemock, seek).Times(1);
		EXPECT_CALL(filemock, read).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(1);
	}

	corejpeg.onDataAvailableCallback(&hjpeg, size);
}

TEST_F(CoreJPEGTest, onDataAvailableCallbackSizeEqual)
{
	JPEG_HandleTypeDef hjpeg;
	uint32_t size {0};

	{
		InSequence seq;
		EXPECT_CALL(filemock, seek).Times(0);
		EXPECT_CALL(filemock, read).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(1);
	}

	corejpeg.onDataAvailableCallback(&hjpeg, size);
}

TEST_F(CoreJPEGTest, onDataAvailableCallbackCannotReadFile)
{
	JPEG_HandleTypeDef hjpeg;
	uint32_t size {2};

	{
		InSequence seq;
		EXPECT_CALL(filemock, seek).Times(1);
		EXPECT_CALL(filemock, read).WillOnce(Return(FR_NO_FILE));
		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(0);
	}

	corejpeg.onDataAvailableCallback(&hjpeg, size);
}

TEST_F(CoreJPEGTest, onDataReadyCallback)
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

TEST_F(CoreJPEGTest, onDecodeCompleteCallback)
{
	JPEG_HandleTypeDef hjpeg;

	corejpeg.onDecodeCompleteCallback(&hjpeg);
}
