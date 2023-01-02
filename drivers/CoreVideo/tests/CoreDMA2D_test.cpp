// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDMA2D.hpp"

#include "gtest/gtest.h"
#include "internal/corevideo_config.h"
#include "mocks/leka/CoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class CoreDMA2DTest : public ::testing::Test
{
  protected:
	CoreDMA2DTest() = default;

	void SetUp() override { setHandleDMA2DReady(); }
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	CoreDMA2D dma2d {halmock};

	void setHandleDMA2DReady()
	{
		auto handle	 = dma2d.getHandle();
		handle.State = HAL_DMA2D_STATE_READY;
		dma2d.setHandle(handle);
	}
};

TEST_F(CoreDMA2DTest, instantiation)
{
	ASSERT_NE(&dma2d, nullptr);
}

TEST_F(CoreDMA2DTest, handleConfigurationInstance)
{
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Instance, DMA2D);
}

TEST_F(CoreDMA2DTest, handleConfigurationInit)
{
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.ColorMode, DMA2D_OUTPUT_ARGB8888);
	ASSERT_EQ(handle.Init.AlphaInverted, DMA2D_REGULAR_ALPHA);
	ASSERT_EQ(handle.Init.RedBlueSwap, DMA2D_RB_REGULAR);
}

TEST_F(CoreDMA2DTest, handleConfigurationLayerConfig)
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

TEST_F(CoreDMA2DTest, initializationSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_DMA2D_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2D_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2D_RELEASE_RESET).Times(1);

		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(DMA2D_IRQn, _, _)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(DMA2D_IRQn)).Times(1);

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1);
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(2);
	}

	dma2d.initialize();
}

TEST_F(CoreDMA2DTest, transferDataSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_Start_IT).Times(1);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(CoreDMA2DTest, transferDataWithFailureForHALDMA2DInit)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_ERROR));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(0);
		EXPECT_CALL(halmock, HAL_DMA2D_Start_IT).Times(0);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(CoreDMA2DTest, transferDataWithFailureForHALDMA2DConfigLayer)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_ERROR));
		EXPECT_CALL(halmock, HAL_DMA2D_Start_IT).Times(0);
	}

	dma2d.transferData(0, 0, 0, 0);
}

TEST_F(CoreDMA2DTest, transferImage)
{
	uint16_t image_width  = 800;
	uint16_t image_height = 480;

	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_Start_IT(_, jpeg::decoded_buffer_address, lcd::frame_buffer_address, image_width,
												image_height))
			.Times(1);
	}

	dma2d.transferImage(image_width, image_height, 100);
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.Mode, DMA2D_M2M_PFC);
	ASSERT_EQ(handle.LayerCfg[1].InputOffset, 100);
	ASSERT_EQ(handle.Init.OutputOffset, lcd::dimension::width - image_width);
}

TEST_F(CoreDMA2DTest, transferImageCentered)
{
	uint16_t image_width  = 123;
	uint16_t image_height = 456;

	auto left_pixels_offset_to_center = (lcd::dimension::width - image_width) / 2;
	auto top_pixel_offset_to_center	  = (lcd::dimension::height - image_height) / 2;

	auto memory_offset_to_center =
		static_cast<unsigned>(lcd::property::pixel_memory_size *
							  (left_pixels_offset_to_center + top_pixel_offset_to_center * lcd::dimension::width));

	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock,
					HAL_DMA2D_Start_IT(_, jpeg::decoded_buffer_address,
									   lcd::frame_buffer_address + memory_offset_to_center, image_width, image_height))
			.Times(1);
	}

	dma2d.transferImage(image_width, image_height, 100);
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.Mode, DMA2D_M2M_PFC);
	ASSERT_EQ(handle.LayerCfg[1].InputOffset, 100);
	ASSERT_EQ(handle.Init.OutputOffset, lcd::dimension::width - image_width);
}

TEST_F(CoreDMA2DTest, transferDrawing)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_DMA2D_Init).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_ConfigLayer).Times(1).WillRepeatedly(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_DMA2D_Start_IT).Times(1);
	}

	dma2d.transferDrawing(0x0, 100, 100, 0xFFFF0000);	// Draw a red square of side 100 pixels in the top left corner
	auto handle = dma2d.getHandle();

	ASSERT_EQ(handle.Init.Mode, DMA2D_R2M);
	ASSERT_EQ(handle.Init.OutputOffset, 700);
}
