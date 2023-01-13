// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreVideo.hpp"

#include "gtest/gtest.h"
#include "mocks/leka/CoreDMA2D.h"
#include "mocks/leka/CoreDSI.h"
#include "mocks/leka/CoreFont.h"
#include "mocks/leka/CoreGraphics.h"
#include "mocks/leka/CoreJPEG.h"
#include "mocks/leka/CoreLCD.h"
#include "mocks/leka/CoreLTDC.h"
#include "mocks/leka/CoreSDRAM.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "mocks/leka/File.h"

using namespace leka;
using ::testing::_;
using ::testing::An;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;

class CoreVideoTest : public ::testing::Test
{
  protected:
	CoreVideoTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	mock::CoreSDRAM sdrammock;
	mock::CoreDMA2D dma2dmock;
	mock::CoreDSI dsimock;
	mock::CoreLTDC ltdcmock;
	mock::CoreLCD lcdmock;
	mock::CoreGraphics graphicsmock;
	mock::CoreFont fontmock;
	mock::CoreJPEG jpegmock;

	mock::File filemock;

	CoreVideo corevideo {halmock, sdrammock, dma2dmock, dsimock, ltdcmock, lcdmock, graphicsmock, fontmock, jpegmock};
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

TEST_F(CoreVideoTest, instantiation)
{
	ASSERT_NE(&corevideo, nullptr);
}

TEST_F(CoreVideoTest, initialization)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_LTDC_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_LTDC_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_LTDC_RELEASE_RESET).Times(1);
		EXPECT_CALL(dsimock, initialize).Times(1);
		EXPECT_CALL(ltdcmock, initialize).Times(1);
		EXPECT_CALL(dsimock, start).Times(1);
		EXPECT_CALL(sdrammock, initialize).Times(1);
		EXPECT_CALL(lcdmock, initialize).Times(1);
		EXPECT_CALL(jpegmock, initialize).Times(1);
		EXPECT_CALL(dma2dmock, initialize).Times(1);
		EXPECT_CALL(lcdmock, setBrightness).Times(1);
	}

	corevideo.initialize();
}

TEST_F(CoreVideoTest, turnOff)
{
	EXPECT_CALL(lcdmock, turnOff).Times(1);

	corevideo.turnOff();
}

TEST_F(CoreVideoTest, turnOn)
{
	EXPECT_CALL(lcdmock, turnOn).Times(1);

	corevideo.turnOn();
}

TEST_F(CoreVideoTest, setBrightness)
{
	EXPECT_CALL(lcdmock, setBrightness(An<float>())).Times(1);

	corevideo.setBrightness(42.F);
}

TEST_F(CoreVideoTest, clearScreenDefaultColor)
{
	EXPECT_CALL(graphicsmock, clearScreen(compareColor(CGColor::white))).Times(1);

	corevideo.clearScreen();
}

TEST_F(CoreVideoTest, clearScreenWithColor)
{
	CGColor clear_color {0x2A, 0x2B, 0x2C};

	EXPECT_CALL(graphicsmock, clearScreen(compareColor(clear_color))).Times(1);

	corevideo.clearScreen(clear_color);
}

TEST_F(CoreVideoTest, drawRectangle)
{
	interface::Graphics::FilledRectangle rectangle;
	rectangle.origin.x = 200;
	rectangle.origin.y = 369;
	rectangle.width	   = 11;
	rectangle.height   = 33;

	CGColor rectangle_color;

	EXPECT_CALL(graphicsmock, drawRectangle(compareFilledRectangle(rectangle), _)).Times(1);

	corevideo.displayRectangle(rectangle, rectangle_color);
}

TEST_F(CoreVideoTest, drawRectangleWithColor)
{
	interface::Graphics::FilledRectangle rectangle;
	CGColor rectangle_color {0x2A, 0x2B, 0x2C};

	EXPECT_CALL(graphicsmock, drawRectangle(_, compareColor(rectangle_color))).Times(1);

	corevideo.displayRectangle(rectangle, rectangle_color);
}

TEST_F(CoreVideoTest, displayImage)
{
	{
		InSequence seq;

		EXPECT_CALL(jpegmock, decodeImage).Times(1);
		EXPECT_CALL(jpegmock, getImageProperties).Times(1);
		EXPECT_CALL(dma2dmock, transferImage).Times(1);
	}

	corevideo.displayImage(filemock);
}

TEST_F(CoreVideoTest, displayImageWithImageProperties)
{
	auto image_properties = JPEGImageProperties {};

	{
		InSequence seq;

		EXPECT_CALL(jpegmock, decodeImage).Times(1);
		EXPECT_CALL(jpegmock, getImageProperties).Times(0);
		EXPECT_CALL(dma2dmock, transferImage).Times(1);
	}

	corevideo.displayImage(filemock, &image_properties);
}

TEST_F(CoreVideoTest, displayText)
{
	constexpr uint8_t buff_size = 128;
	char buff[buff_size] {};
	auto text_length = snprintf(buff, buff_size, "Some text");

	auto starting_line = 1;

	EXPECT_CALL(fontmock, display(buff, text_length, starting_line, _, _)).Times(1);

	corevideo.displayText(buff, text_length, starting_line);
}

TEST_F(CoreVideoTest, displayTextWithColor)
{
	constexpr uint8_t buff_size = 128;
	char buff[buff_size] {};
	auto text_length = snprintf(buff, buff_size, "Some text");

	auto starting_line = 1;

	CGColor foreground_color;
	CGColor background_color;

	EXPECT_CALL(fontmock, display(_, _, _, compareColor(foreground_color), compareColor(background_color))).Times(1);

	corevideo.displayText(buff, text_length, starting_line, foreground_color, background_color);
}

TEST_F(CoreVideoTest, setVideo)
{
	const auto start_of_file_index = 0;
	const auto any_frame_index	   = 5668;

	{
		InSequence seq;

		EXPECT_CALL(jpegmock, findSOIMarker(_, start_of_file_index)).WillOnce(Return(any_frame_index));
		EXPECT_CALL(filemock, seek(any_frame_index, SEEK_SET));
		EXPECT_CALL(jpegmock, decodeImage).Times(1);
		EXPECT_CALL(jpegmock, getImageProperties);
	}

	corevideo.setVideo(filemock);

	auto actual_is_last_frame = corevideo.isLastFrame();
	EXPECT_FALSE(actual_is_last_frame);
}

TEST_F(CoreVideoTest, displayNextFrameVideo)
{
	const auto any_frame_index = 218;

	{
		InSequence seq;

		EXPECT_CALL(jpegmock, findSOIMarker).WillOnce(Return(any_frame_index));
		EXPECT_CALL(filemock, seek(any_frame_index, SEEK_SET));

		EXPECT_CALL(jpegmock, decodeImage).Times(1);
		EXPECT_CALL(dma2dmock, transferImage).Times(1);
	}

	corevideo.displayNextFrameVideo(filemock);

	auto actual_is_last_frame = corevideo.isLastFrame();
	EXPECT_FALSE(actual_is_last_frame);
}

TEST_F(CoreVideoTest, displayNextFrameVideoIsLastFrame)
{
	{
		InSequence seq;

		EXPECT_CALL(jpegmock, findSOIMarker).WillOnce(Return(0));
		EXPECT_CALL(filemock, seek).Times(0);

		EXPECT_CALL(jpegmock, decodeImage).Times(0);
		EXPECT_CALL(dma2dmock, transferImage).Times(0);
	}

	corevideo.displayNextFrameVideo(filemock);

	auto actual_is_last_frame = corevideo.isLastFrame();
	EXPECT_TRUE(actual_is_last_frame);
}

TEST_F(CoreVideoTest, getDMA2DHandle)
{
	auto expected_handle = DMA2D_HandleTypeDef {};
	EXPECT_CALL(dma2dmock, getHandle).WillOnce(ReturnRef(expected_handle));

	corevideo.getDMA2DHandle();
}

TEST_F(CoreVideoTest, getLTDCHandle)
{
	auto expected_handle = LTDC_HandleTypeDef {};
	EXPECT_CALL(ltdcmock, getHandle).WillOnce(ReturnRef(expected_handle));

	corevideo.getLTDCHandle();
}

TEST_F(CoreVideoTest, getJPEGHandle)
{
	auto expected_handle = JPEG_HandleTypeDef {};
	EXPECT_CALL(jpegmock, getHandle).WillOnce(ReturnRef(expected_handle));

	corevideo.getJPEGHandle();
}
