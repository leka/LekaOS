// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreDMA2D.h"

#include "corevideo_config.h"
#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class LKCoreDMA2DTest : public ::testing::Test
{
  protected:
	LKCoreDMA2DTest() : dma2d(hal) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreSTM32HalMock hal;
	LKCoreDMA2D dma2d;
};

TEST_F(LKCoreDMA2DTest, instantiation)
{
	ASSERT_NE(&dma2d, nullptr);
}

TEST_F(LKCoreDMA2DTest, handleConfigurationInstance)
{
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Instance, DMA2D);
}

TEST_F(LKCoreDMA2DTest, handleConfigurationInit)
{
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.ColorMode, DMA2D_OUTPUT_ARGB8888);
	ASSERT_EQ(handle.Init.AlphaInverted, DMA2D_REGULAR_ALPHA);
	ASSERT_EQ(handle.Init.RedBlueSwap, DMA2D_RB_REGULAR);
}

TEST_F(LKCoreDMA2DTest, handleConfigurationLayerConfig)
{
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.LayerCfg[0].AlphaMode, DMA2D_REPLACE_ALPHA);
	ASSERT_EQ(handle.LayerCfg[0].InputAlpha, 0xFF);
	ASSERT_EQ(handle.LayerCfg[0].InputColorMode, DMA2D_INPUT_ARGB8888);
	ASSERT_EQ(handle.LayerCfg[0].RedBlueSwap, DMA2D_RB_REGULAR);
	ASSERT_EQ(handle.LayerCfg[0].AlphaInverted, DMA2D_REGULAR_ALPHA);

	ASSERT_EQ(handle.LayerCfg[1].AlphaMode, DMA2D_REPLACE_ALPHA);
	ASSERT_EQ(handle.LayerCfg[1].InputAlpha, 0xFF);
	ASSERT_EQ(handle.LayerCfg[1].InputColorMode, DMA2D_INPUT_ARGB8888);
	ASSERT_EQ(handle.LayerCfg[1].RedBlueSwap, DMA2D_RB_REGULAR);
	ASSERT_EQ(handle.LayerCfg[1].AlphaInverted, DMA2D_REGULAR_ALPHA);
}

TEST_F(LKCoreDMA2DTest, initializationSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1);
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(2);
	}

	dma2d.initialize();
}

TEST_F(LKCoreDMA2DTest, transferDataSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_Start).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(1);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(LKCoreDMA2DTest, transferDataWithFailureForHALDMA2DInit)
{
	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_ERROR));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(0);
		EXPECT_CALL(hal, HAL_DMA2D_Start).Times(0);
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(0);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(LKCoreDMA2DTest, transferDataWithFailureForHALDMA2DConfigLayer)
{
	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_ERROR));
		EXPECT_CALL(hal, HAL_DMA2D_Start).Times(0);
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(0);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(LKCoreDMA2DTest, transferDataWithFailureForHALDMA2DStart)
{
	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_Start).Times(1).WillRepeatedly(Return(HAL_ERROR));
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(0);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(LKCoreDMA2DTest, transferImage)
{
	uint16_t image_width  = 800;
	uint16_t image_height = 480;

	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_Start(_, jpeg::decoded_buffer_address, lcd::frame_buffer_address, image_width, image_height))
			.Times(1)
			.WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(1);
	}

	dma2d.transferImage(image_width, image_height, 100);
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.Mode, DMA2D_M2M_PFC);
	ASSERT_EQ(handle.LayerCfg[1].InputOffset, 100);
	ASSERT_EQ(handle.Init.OutputOffset, 0);
}

TEST_F(LKCoreDMA2DTest, transferDrawing)
{
	{
		InSequence seq;

		EXPECT_CALL(hal, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_Start).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(hal, HAL_DMA2D_PollForTransfer).Times(1);
	}

	dma2d.transferDrawing(0x0, 100, 100, 0xFFFF0000);	// Draw a red square of side 100 pixels in the top left corner
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.Mode, DMA2D_R2M);
	ASSERT_EQ(handle.Init.OutputOffset, 700);
}
