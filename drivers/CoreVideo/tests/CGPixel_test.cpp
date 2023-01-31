// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CGGraphics.hpp"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLL.h"

using namespace leka;

TEST(CGPixelTest, draw)
{
	mock::CoreLLMock llmock;
	CGPixel pixel(llmock);
	pixel.coordinates.x = 42;
	pixel.coordinates.y = 99;
	CGColor color		= CGColor::magenta;

	uintptr_t expected_address =
		lcd::frame_buffer_address + (4 * (pixel.coordinates.y * lcd::dimension::width + pixel.coordinates.x));
	auto expected_data = color.getARGB();

	EXPECT_CALL(llmock, rawMemoryWrite(expected_address, expected_data));

	pixel.draw(color);
}
