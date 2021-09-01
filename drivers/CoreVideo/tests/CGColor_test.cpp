// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CGColor.hpp"

#include "gtest/gtest.h"

using namespace leka;

TEST(CGColorTest, getARGBFromAnyColor)
{
	CGColor color;
	color.alpha = 0x2A;
	color.red	= 0x2B;
	color.green = 0x2C;
	color.blue	= 0x2D;

	ASSERT_EQ(0x2A2B2C2D, color.getARGB());
}

TEST(CGColorTest, getARGBRegisteredColor)
{
	CGColor magenta = CGColor::magenta;

	auto expected_value = magenta.alpha << 24 | magenta.red << 16 | magenta.green << 8 | magenta.blue;

	ASSERT_EQ(expected_value, magenta.getARGB());
}
