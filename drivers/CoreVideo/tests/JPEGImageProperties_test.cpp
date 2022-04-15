// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "JPEGImageProperties.hpp"

#include "gtest/gtest.h"

using namespace leka;

TEST(JPEGImageProperties, getWidthOffsetUndefinedChromaSubsampling)
{
	auto image_properties			   = JPEGImageProperties {};
	image_properties.ChromaSubsampling = 3;

	auto expected_width_offset = 0;

	auto actual_width_offset = image_properties.getWidthOffset();

	EXPECT_EQ(actual_width_offset, expected_width_offset);
}

TEST(JPEGImageProperties, getWidthOffsetChromaSubsampling420)
{
	auto image_properties			   = JPEGImageProperties {};
	image_properties.ChromaSubsampling = JPEG_420_SUBSAMPLING;

	for (int offset = 0; offset < 16; offset++) {
		// Setup
		image_properties.ImageWidth = 16 * 50 + offset;	  // =800 + i

		auto expected_width_offset = (16 - offset) % 16;

		// Test
		auto actual_width_offset = image_properties.getWidthOffset();
		EXPECT_EQ(actual_width_offset, expected_width_offset);
	}
}

TEST(JPEGImageProperties, getWidthOffsetChromaSubsampling422)
{
	auto image_properties			   = JPEGImageProperties {};
	image_properties.ChromaSubsampling = JPEG_422_SUBSAMPLING;

	for (int offset = 0; offset < 16; offset++) {
		// Setup
		image_properties.ImageWidth = 16 * 50 + offset;	  // =800 + i

		auto expected_width_offset = (16 - offset) % 16;

		// Test
		auto actual_width_offset = image_properties.getWidthOffset();
		EXPECT_EQ(actual_width_offset, expected_width_offset);
	}
}

TEST(JPEGImageProperties, getWidthOffsetChromaSubsampling444)
{
	auto image_properties			   = JPEGImageProperties {};
	image_properties.ChromaSubsampling = JPEG_444_SUBSAMPLING;

	for (int offset = 0; offset < 8; offset++) {
		// Setup
		image_properties.ImageWidth = 8 * 100 + offset;	  // =800 + i

		auto expected_width_offset = offset % 8;

		// Test
		auto actual_width_offset = image_properties.getWidthOffset();
		EXPECT_EQ(actual_width_offset, expected_width_offset);
	}
}
