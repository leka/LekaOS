// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RGB.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(RGBTest, equal)
{
	RGB black = RGB::black;
	RGB color = RGB {255, 215, 00};

	EXPECT_TRUE(black == RGB::black);
	EXPECT_TRUE((black == RGB {0x00, 0x00, 0x00}));

	EXPECT_TRUE((color == RGB {255, 215, 0}));
}

TEST(RGBTest, different)
{
	RGB black = RGB::black;
	RGB color = RGB {255, 215, 00};

	EXPECT_TRUE(RGB::black != RGB::white);
	EXPECT_TRUE((black != RGB {0xFF, 0xFF, 0xFF}));
	EXPECT_FALSE((black == RGB {0xFF, 0xFF, 0xFF}));
}

TEST(RGBTest, addTwoColor)
{
	RGB color1 = RGB::pure_red;
	color1 += RGB::pure_blue;

	RGB color2 = RGB::pure_red;
	color2 += RGB::pure_green;

	RGB color3 = RGB::pure_green;
	color3 += RGB::pure_red;

	RGB color4 = RGB::pure_green;
	color4 += RGB::pure_blue;

	RGB color5 = RGB::pure_blue;
	color5 += RGB::pure_red;

	RGB color6 = RGB::pure_blue;
	color6 += RGB::pure_green;

	RGB color7 = RGB::pure_blue;
	color7 += RGB::pure_green;
	color7 += RGB::pure_red;

	EXPECT_TRUE(color1 == RGB::magenta);
	EXPECT_TRUE(color2 == RGB::yellow);
	EXPECT_TRUE(color3 == RGB::yellow);
	EXPECT_TRUE(color4 == RGB::cyan);
	EXPECT_TRUE(color5 == RGB::magenta);
	EXPECT_TRUE(color6 == RGB::cyan);
	EXPECT_TRUE(color7 == RGB::white);
}

TEST(RGBTest, addTwoColorOverTheBound1)
{
	RGB color = RGB::pure_red;
	color += RGB::pure_red;

	EXPECT_TRUE(color == RGB::pure_red);
}

TEST(RGBTest, addTwoColorOverTheBound2)
{
	RGB color = RGB::white;
	color += RGB {10, 7, 120};

	EXPECT_TRUE(color == RGB::white);
}

TEST(RGBTest, sumTwoColor)
{
	auto magenta1 = RGB::pure_red + RGB::pure_blue;
	EXPECT_TRUE(magenta1 == RGB::magenta);

	RGB magenta2 = RGB::pure_blue + RGB::pure_red;
	EXPECT_TRUE(magenta2 == RGB::magenta);

	auto yellow1 = RGB::pure_red + RGB::pure_green;
	EXPECT_TRUE(yellow1 == RGB::yellow);

	auto yellow2 = RGB::pure_green + RGB::pure_red;
	EXPECT_TRUE(yellow2 == RGB::yellow);

	auto cyan1 = RGB::pure_green + RGB::pure_blue;
	EXPECT_TRUE(cyan1 == RGB::cyan);

	auto cyan2 = RGB::pure_blue + RGB::pure_green;
	EXPECT_TRUE(cyan2 == RGB::cyan);

	auto white = RGB::pure_red + RGB::pure_green + RGB::pure_blue;
	EXPECT_TRUE(white == RGB::white);
}
