// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreGraphics.h"

#include "corevideo_config.h"
#include "gtest/gtest.h"
#include "mock_LKCoreDMA2D.h"

using namespace leka;
using ::testing::_;

class LKCoreGraphicsTest : public ::testing::Test
{
  protected:
	LKCoreGraphicsTest() : graphics(dma2dmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreDMA2DMock dma2dmock;
	LKCoreGraphics graphics;
};

TEST_F(LKCoreGraphicsTest, instantiation)
{
	ASSERT_NE(&graphics, nullptr);
}

TEST_F(LKCoreGraphicsTest, drawRectangle)
{
	// ENHANCEMENT: Do for any origin and any width/height

	auto n_channel			   = 4;
	auto starting_pixel_column = 10;
	auto starting_pixel_line   = 20;
	auto rectangle_width	   = 30;
	auto rectangle_height	   = 40;
	auto expected_color		   = CGColor::magenta;

	LKCoreGraphics::FilledRectangle rectangle;
	rectangle.origin.x = starting_pixel_column;
	rectangle.origin.y = starting_pixel_line;
	rectangle.width	   = rectangle_width;
	rectangle.height   = rectangle_height;

	uintptr_t expected_address =
		lcd::frame_buffer_address + n_channel * (lcd::dimension.width * starting_pixel_line + starting_pixel_column);

	EXPECT_CALL(dma2dmock,
				transferDrawing(expected_address, rectangle_width, rectangle_height, expected_color.getARGB()))
		.Times(1);

	graphics.drawRectangle(rectangle, CGColor::magenta);
}

TEST_F(LKCoreGraphicsTest, clearScreenDefaultColor)
{
	uintptr_t expected_address = lcd::frame_buffer_address + 0;
	auto expected_color		   = CGColor::white;

	EXPECT_CALL(dma2dmock, transferDrawing(expected_address, lcd::dimension.width, lcd::dimension.height,
										   expected_color.getARGB()))
		.Times(1);

	graphics.clearScreen();
}

TEST_F(LKCoreGraphicsTest, clearScreenOtherColor)
{
	uintptr_t expected_address = lcd::frame_buffer_address + 0;
	auto expected_color		   = CGColor::magenta;

	EXPECT_CALL(dma2dmock, transferDrawing(expected_address, lcd::dimension.width, lcd::dimension.height,
										   expected_color.getARGB()))
		.Times(1);

	graphics.clearScreen(CGColor::magenta);
}
