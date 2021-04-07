// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreVideo.h"

#include "gtest/gtest.h"
#include "mock_LKCoreDMA2D.h"
#include "mock_LKCoreDSI.h"
#include "mock_LKCoreFont.h"
#include "mock_LKCoreGraphics.h"
#include "mock_LKCoreJPEG.h"
#include "mock_LKCoreLCD.h"
#include "mock_LKCoreLTDC.h"
#include "mock_LKCoreSDRAM.h"
#include "mock_LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;

class LKCoreVideoTest : public ::testing::Test
{
  protected:
	LKCoreVideoTest()
		: corevideo(halmock, sdrammock, dma2dmock, dsimock, ltdcmock, lcdmock, graphicsmock, fontmock, jpegmock)
	{
	}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreSTM32HalMock halmock;
	LKCoreSDRAMMock sdrammock;
	LKCoreDMA2DMock dma2dmock;
	LKCoreDSIMock dsimock;
	LKCoreLTDCMock ltdcmock;
	LKCoreLCDMock lcdmock;
	LKCoreGraphicsMock graphicsmock;
	LKCoreFontMock fontmock;
	LKCoreJPEGMock jpegmock;

	LKCoreVideo corevideo;
};

MATCHER_P(compareColor, expected_color, "")
{
	bool same_red	= arg.red == expected_color.red;
	bool same_green = arg.green == expected_color.green;
	bool same_blue	= arg.blue == expected_color.blue;
	return (same_red && same_green && same_blue);
}

MATCHER_P(compareFilledRectangle, expected_rectangle, "")
{
	bool same_origin_x = arg.origin.x == expected_rectangle.origin.x;
	bool same_origin_y = arg.origin.y == expected_rectangle.origin.y;
	bool same_origin   = (same_origin_x && same_origin_y);

	bool same_width		 = arg.width == expected_rectangle.width;
	bool same_height	 = arg.height == expected_rectangle.height;
	bool same_dimensions = (same_width && same_height);

	return (same_origin && same_dimensions);
}

TEST_F(LKCoreVideoTest, instantiation)
{
	ASSERT_NE(&corevideo, nullptr);
}

TEST_F(LKCoreVideoTest, initialization)
{
	{
		InSequence seq;

		EXPECT_CALL(dsimock, reset).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_LTDC_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_LTDC_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_LTDC_RELEASE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2D_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2D_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2D_RELEASE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DSI_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DSI_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DSI_RELEASE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(LTDC_IRQn, _, _)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(LTDC_IRQn)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(DMA2D_IRQn, _, _)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(DMA2D_IRQn)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(DSI_IRQn, _, _)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(DSI_IRQn)).Times(1);
		EXPECT_CALL(dsimock, initialize).Times(1);
		EXPECT_CALL(ltdcmock, initialize).Times(1);
		EXPECT_CALL(ltdcmock, configureLayer).Times(1);
		EXPECT_CALL(dsimock, start).Times(1);
		EXPECT_CALL(sdrammock, initialize).Times(1);
		EXPECT_CALL(lcdmock, initialize).Times(1);
		EXPECT_CALL(jpegmock, initialize).Times(1);
		EXPECT_CALL(dma2dmock, initialize).Times(1);
		EXPECT_CALL(lcdmock, setBrightness).Times(1);
	}

	corevideo.initialize();
}

TEST_F(LKCoreVideoTest, turnOff)
{
	EXPECT_CALL(lcdmock, turnOff).Times(1);

	corevideo.turnOff();
}

TEST_F(LKCoreVideoTest, turnOn)
{
	EXPECT_CALL(lcdmock, turnOn).Times(1);

	corevideo.turnOn();
}

TEST_F(LKCoreVideoTest, setBrightness)
{
	float brightness_value;

	EXPECT_CALL(lcdmock, setBrightness(brightness_value)).Times(1);

	corevideo.setBrightness(brightness_value);
}

TEST_F(LKCoreVideoTest, clearScreen)
{
	EXPECT_CALL(graphicsmock, clearScreen).Times(1);

	corevideo.clearScreen();
}

TEST_F(LKCoreVideoTest, clearScreenWithColor)
{
	Color clear_color {0x2A, 0x2B, 0x2C};

	EXPECT_CALL(graphicsmock, clearScreen(compareColor(clear_color))).Times(1);

	corevideo.clearScreen(clear_color);
}

TEST_F(LKCoreVideoTest, drawRectangle)
{
	LKCoreGraphicsBase::FilledRectangle rectangle;
	rectangle.origin.x = 200;
	rectangle.origin.y = 369;
	rectangle.width	   = 11;
	rectangle.height   = 33;

	Color rectangle_color;

	EXPECT_CALL(graphicsmock, drawRectangle(compareFilledRectangle(rectangle), _)).Times(1);

	corevideo.displayRectangle(rectangle, rectangle_color);
}

TEST_F(LKCoreVideoTest, drawRectangleWithColor)
{
	LKCoreGraphicsBase::FilledRectangle rectangle;
	Color rectangle_color {0x2A, 0x2B, 0x2C};

	EXPECT_CALL(graphicsmock, drawRectangle(_, compareColor(rectangle_color))).Times(1);

	corevideo.displayRectangle(rectangle, rectangle_color);
}

TEST_F(LKCoreVideoTest, displayImage)
{
	FIL file;
	EXPECT_CALL(jpegmock, displayImage(&file)).Times(1);

	corevideo.displayImage(&file);
}

TEST_F(LKCoreVideoTest, displayText)
{
	constexpr uint8_t buff_size = 128;
	char buff[buff_size] {};
	auto text_length = sprintf(buff, "Some text");

	auto starting_line = 1;

	EXPECT_CALL(fontmock, display(buff, text_length, starting_line, _, _)).Times(1);

	corevideo.displayText(buff, text_length, starting_line);
}

TEST_F(LKCoreVideoTest, displayTextWithColor)
{
	constexpr uint8_t buff_size = 128;
	char buff[buff_size] {};
	auto text_length = sprintf(buff, "Some text");

	auto starting_line = 1;

	Color foreground_color;
	Color background_color;

	EXPECT_CALL(fontmock, display(_, _, _, compareColor(foreground_color), compareColor(background_color))).Times(1);

	corevideo.displayText(buff, text_length, starting_line, foreground_color, background_color);
}
