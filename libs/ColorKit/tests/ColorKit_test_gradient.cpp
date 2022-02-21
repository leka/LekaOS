// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "ColorKit.h"
#include "gtest/gtest.h"

using namespace leka;

static constexpr uint8_t number_of_leds = 20;

TEST(ColorKitGradientTest, gradientWhiteToBlack)
{
	std::array<RGB, number_of_leds> colors {};
	auto expected_colors = std::to_array<RGB>(
		{RGB::white,		  RGB {217, 217, 217}, RGB {183, 183, 183}, RGB {152, 152, 152}, RGB {125, 125, 125},
		 RGB {102, 102, 102}, RGB {82, 82, 82},	   RGB {64, 64, 64},	RGB {49, 49, 49},	 RGB {37, 37, 37},
		 RGB {27, 27, 27},	  RGB {19, 19, 19},	   RGB {13, 13, 13},	RGB {8, 8, 8},		 RGB {5, 5, 5},
		 RGB {2, 2, 2},		  RGB {1, 1, 1},	   RGB {0, 0, 0},		RGB {0, 0, 0},		 RGB::black});

	for (auto i = 0; i < number_of_leds; i++) {
		RGB col_rgb	 = ColorKit::colorGradient(RGB::white, RGB::black, static_cast<float>(i) / (number_of_leds - 1));
		colors.at(i) = col_rgb;
	}

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), colors.begin()));
}

TEST(ColorKitGradientTest, gradientWhiteToPureRed)
{
	std::array<RGB, number_of_leds> colors {};
	auto expected_colors = std::to_array<RGB>(
		{RGB::white,		  RGB {255, 234, 229}, RGB {255, 213, 205}, RGB {255, 194, 183}, RGB {255, 176, 162},
		 RGB {255, 158, 142}, RGB {255, 142, 124}, RGB {255, 126, 108}, RGB {255, 111, 93},	 RGB {255, 98, 79},
		 RGB {255, 84, 66},	  RGB {255, 72, 55},   RGB {255, 61, 45},	RGB {255, 50, 35},	 RGB {255, 40, 27},
		 RGB {255, 30, 20},	  RGB {255, 22, 14},   RGB {255, 14, 8},	RGB {255, 7, 4},	 RGB::pure_red});

	for (auto i = 0; i < number_of_leds; i++) {
		RGB col_rgb	 = ColorKit::colorGradient(RGB::white, RGB::pure_red, static_cast<float>(i) / (number_of_leds - 1));
		colors.at(i) = col_rgb;
	}

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), colors.begin()));
}

TEST(ColorKitGradientTest, gradientWhiteToPureGreen)
{
	std::array<RGB, number_of_leds> colors {};
	auto expected_colors = std::to_array<RGB>(
		{RGB::white,		  RGB {237, 255, 235}, RGB {220, 255, 216}, RGB {203, 255, 198}, RGB {187, 255, 180},
		 RGB {172, 255, 164}, RGB {156, 255, 148}, RGB {142, 255, 132}, RGB {127, 255, 118}, RGB {114, 255, 104},
		 RGB {100, 255, 91},  RGB {87, 255, 78},   RGB {75, 255, 66},	RGB {63, 255, 55},	 RGB {52, 255, 44},
		 RGB {40, 255, 34},	  RGB {30, 255, 25},   RGB {19, 255, 16},	RGB {9, 255, 8},	 RGB::pure_green});

	for (auto i = 0; i < number_of_leds; i++) {
		RGB col_rgb =
			ColorKit::colorGradient(RGB::white, RGB::pure_green, static_cast<float>(i) / (number_of_leds - 1));
		colors.at(i) = col_rgb;
	}

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), colors.begin()));
}

TEST(ColorKitGradientTest, gradientWhiteToRandomColor)
{
	std::array<RGB, number_of_leds> colors {};
	auto expected_colors = std::to_array<RGB>(
		{RGB::white,		  RGB {255, 254, 236}, RGB {254, 252, 218}, RGB {254, 250, 200}, RGB {254, 249, 183},
		 RGB {254, 247, 167}, RGB {253, 245, 151}, RGB {253, 243, 136}, RGB {253, 241, 122}, RGB {253, 239, 108},
		 RGB {253, 237, 95},  RGB {253, 235, 82},  RGB {253, 232, 70},	RGB {254, 230, 59},	 RGB {254, 228, 48},
		 RGB {254, 225, 37},  RGB {254, 223, 27},  RGB {254, 220, 18},	RGB {255, 218, 9},	 RGB {255, 215, 0}});

	for (auto i = 0; i < number_of_leds; i++) {
		RGB col_rgb =
			ColorKit::colorGradient(RGB::white, RGB {255, 215, 0}, static_cast<float>(i) / (number_of_leds - 1));
		colors.at(i) = col_rgb;
	}

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), colors.begin()));
}

TEST(ColorKitGradientTest, gradientRandomColorToRandomColor)
{
	std::array<RGB, number_of_leds> colors {};
	auto expected_colors = std::to_array<RGB>(
		{RGB {67, 17, 180},	 RGB {72, 24, 172},	 RGB {77, 31, 165},	 RGB {83, 38, 157},	 RGB {89, 46, 149},
		 RGB {96, 54, 141},	 RGB {103, 63, 132}, RGB {111, 71, 123}, RGB {119, 81, 115}, RGB {128, 91, 105},
		 RGB {138, 101, 96}, RGB {148, 112, 86}, RGB {159, 123, 77}, RGB {171, 134, 67}, RGB {183, 147, 56},
		 RGB {196, 159, 46}, RGB {210, 172, 35}, RGB {224, 186, 23}, RGB {239, 200, 12}, RGB {255, 215, 0}});

	for (auto i = 0; i < number_of_leds; i++) {
		RGB col_rgb =
			ColorKit::colorGradient(RGB {67, 17, 180}, RGB {255, 215, 0}, static_cast<float>(i) / (number_of_leds - 1));
		colors.at(i) = col_rgb;
	}

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), colors.begin()));
}
