// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEGModeDMA.hpp"

// #include "external/st_jpeg_utils.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "mocks/leka/File.h"

using namespace leka;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::Return;

class CoreJPEGModeDMATest : public ::testing::Test
{
  protected:
	CoreJPEGModeDMATest() : corejpegmode(halmock) {}

	void SetUp() override
	{
		// EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).Times(1);
		// corejpegmode.decode(&hjpeg, &filemock);
	}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	mock::File filemock;
	CoreJPEGModeDMA corejpegmode;

	JPEG_HandleTypeDef hjpeg;
};

TEST_F(CoreJPEGModeDMATest, instantiation)
{
	EXPECT_NE(&corejpegmode, nullptr);
}

TEST_F(CoreJPEGModeDMATest, onMspInitCallback)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_DMA2_CLK_ENABLE).Times(1);

		EXPECT_CALL(halmock, HAL_DMA_DeInit).Times(1);
		EXPECT_CALL(halmock, HAL_DMA_Init).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);

		EXPECT_CALL(halmock, HAL_DMA_DeInit).Times(1);
		EXPECT_CALL(halmock, HAL_DMA_Init).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);
	}

	corejpegmode.onMspInitCallback(&hjpeg);
}

TEST_F(CoreJPEGModeDMATest, onErrorCallback)
{
	corejpegmode.onErrorCallback(&hjpeg);

	// nothing expected
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback420ChromSubsamplingDimensionsMultiple)
{
	auto config = JPEG_ConfTypeDef {
		.ChromaSubsampling = JPEG_420_SUBSAMPLING,
		.ImageHeight	   = 16 * 30,	// = 480
		.ImageWidth		   = 16 * 50,	// = 800
	};

	corejpegmode.onInfoReadyCallback(&hjpeg, &config);

	EXPECT_EQ(config.ImageWidth, 800);
	EXPECT_EQ(config.ImageHeight, 480);
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback420ChromSubsamplingDimensionsNotMultiple)
{
	auto config = JPEG_ConfTypeDef {.ChromaSubsampling = JPEG_420_SUBSAMPLING};

	for (int i = 1; i < 16; i++) {
		config.ImageWidth  = static_cast<uint32_t>(16 * 50 + i);   // = 800 + i
		config.ImageHeight = static_cast<uint32_t>(16 * 30 + i);   // = 480 + i

		corejpegmode.onInfoReadyCallback(&hjpeg, &config);

		const auto width_ceil_multiple	= 800 + 16 * ((15 + i) / 16);
		const auto height_ceil_multiple = 480 + 16 * ((15 + i) / 16);

		EXPECT_EQ(config.ImageWidth, width_ceil_multiple);
		EXPECT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback422ChromSubsamplingDimensionsMultiple)
{
	auto config = JPEG_ConfTypeDef {
		.ChromaSubsampling = JPEG_422_SUBSAMPLING,
		.ImageHeight	   = 8 * 60,	// = 480
		.ImageWidth		   = 16 * 50,	// = 800
	};

	corejpegmode.onInfoReadyCallback(&hjpeg, &config);

	EXPECT_EQ(config.ImageWidth, 800);
	EXPECT_EQ(config.ImageHeight, 480);
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback422ChromSubsamplingDimensionsNotMultiple)
{
	auto config = JPEG_ConfTypeDef {.ChromaSubsampling = JPEG_422_SUBSAMPLING};

	for (int i = 1; i < 16; i++) {
		config.ImageWidth  = static_cast<uint32_t>(16 * 50 + i);   // = 800 + i
		config.ImageHeight = static_cast<uint32_t>(8 * 60 + i);	   // = 480 + i

		corejpegmode.onInfoReadyCallback(&hjpeg, &config);

		auto width_ceil_multiple  = 800 + 16 * ((15 + i) / 16);
		auto height_ceil_multiple = 480 + 8 * ((7 + i) / 8);

		EXPECT_EQ(config.ImageWidth, width_ceil_multiple);
		EXPECT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback444ChromSubsamplingDimensionsMultiple)
{
	auto config = JPEG_ConfTypeDef {
		.ChromaSubsampling = JPEG_444_SUBSAMPLING,
		.ImageHeight	   = 8 * 60,	// = 480
		.ImageWidth		   = 8 * 100,	// = 800
	};

	corejpegmode.onInfoReadyCallback(&hjpeg, &config);

	EXPECT_EQ(config.ImageWidth, 800);
	EXPECT_EQ(config.ImageHeight, 480);
}

TEST_F(CoreJPEGModeDMATest, onInfoReadyCallback444ChromSubsamplingDimensionsNotMultiple)
{
	auto config = JPEG_ConfTypeDef {.ChromaSubsampling = JPEG_444_SUBSAMPLING};

	for (int i = 1; i < 8; i++) {
		config.ImageWidth  = static_cast<uint32_t>(8 * 100 + i);   // = 800 + i
		config.ImageHeight = static_cast<uint32_t>(8 * 60 + i);	   // = 480 + i

		corejpegmode.onInfoReadyCallback(&hjpeg, &config);

		auto width_ceil_multiple  = 800 + 8 * ((7 + i) / 8);
		auto height_ceil_multiple = 480 + 8 * ((7 + i) / 8);

		EXPECT_EQ(config.ImageWidth, width_ceil_multiple);
		EXPECT_EQ(config.ImageHeight, height_ceil_multiple);
	}
}

TEST_F(CoreJPEGModeDMATest, onDataReadyCallback)
{
	auto pDataOut = uint8_t {0};
	auto size	  = uint32_t {0};

	EXPECT_CALL(halmock, HAL_JPEG_ConfigOutputBuffer).Times(1);

	corejpegmode.onDataReadyCallback(&hjpeg, &pDataOut, size);
}

// TEST_F(CoreJPEGModeDMATest, onDataAvailableCallback)
// {
// 	auto size = uint32_t {2};

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(filemock, seek).Times(1);
// 		EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(42));
// 		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(1);
// 	}

// 	corejpegmode.onDataAvailableCallback(&hjpeg, size);
// }

// TEST_F(CoreJPEGModeDMATest, onDataAvailableCallbackSizeEqual)
// {
// 	auto size = uint32_t {42};

// 	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(42));
// 	EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(1);
// 	corejpegmode.decode(&hjpeg, &filemock);

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(filemock, seek).Times(0);
// 		EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(42));
// 		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(1);
// 	}

// 	corejpegmode.onDataAvailableCallback(&hjpeg, size);
// }

// TEST_F(CoreJPEGModeDMATest, onDataAvailableCallbackCannotReadFile)
// {
// 	auto size = uint32_t {2};

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(filemock, seek).Times(1);
// 		EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(0));
// 		EXPECT_CALL(halmock, HAL_JPEG_ConfigInputBuffer).Times(0);
// 	}

// 	corejpegmode.onDataAvailableCallback(&hjpeg, size);
// }

// TEST_F(CoreJPEGModeDMATest, decodeSuccess)
// {
// 	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(42));
// 	EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(1);

// 	auto status = corejpegmode.decode(&hjpeg, &filemock);

// 	EXPECT_EQ(status, HAL_OK);
// }

// TEST_F(CoreJPEGModeDMATest, decodeFailed)
// {
// 	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillOnce(Return(0));
// 	EXPECT_CALL(halmock, HAL_JPEG_Decode).Times(0);

// 	auto status = corejpegmode.decode(&hjpeg, &filemock);

// 	EXPECT_NE(status, HAL_OK);
// }

TEST_F(CoreJPEGModeDMATest, onDecodeCompleteCallback)
{
	corejpegmode.onDecodeCompleteCallback(&hjpeg);

	// nothing expected
}
