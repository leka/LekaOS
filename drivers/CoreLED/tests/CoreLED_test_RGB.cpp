// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RGB.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(RGBTest, equalOperator)
{
	RGB black = RGB::black;
	RGB color = RGB {255, 215, 00};

	EXPECT_TRUE((black == RGB::black));
	EXPECT_TRUE((black == RGB {0x00, 0x00, 0x00}));

	EXPECT_TRUE((color == RGB {255, 215, 0}));

	EXPECT_FALSE((black == RGB {0xFF, 0xFF, 0xFF}));
}
