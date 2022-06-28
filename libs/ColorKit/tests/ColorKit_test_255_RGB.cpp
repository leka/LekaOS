// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "ColorKit.h"
#include "LogKit.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(ColorKitCreate255RGB, paletteWhiteAndBlack)
{
	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {255, 255, 255}}, {255, {0, 0, 0}}});

	std::array<RGB, 255> expected_colors {};
	for (int i(1); i <= 255; ++i) {
		expected_colors[i - 1] =
			// RGB {static_cast<uint8_t>(255 - i), static_cast<uint8_t>(255 - i), static_cast<uint8_t>(255 - i)};
			RGB {0, 0, 0};
	}
	std::array<RGB, 255> output_gradient {};
	std::span<RGB> output_span(output_gradient);

	leka::ColorKit::createGradientRGB255(gradient_colors, RGB::white, RGB::black, output_gradient);
	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
	// EXPECT_EQ(expected_colors[0], output_gradient[0]);
}

// TEST(ColorKitCreate255RGB, vectorBlackToWhite)
// {
// 	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {0, 0, 0}}, {255, {255, 255, 255}}});

// 	std::vector<RGB> expected_colors {};
// 	for (int i(0); i < 255; ++i) {
// 		expected_colors.push_back(RGB {static_cast<uint8_t>(i), static_cast<uint8_t>(i), static_cast<uint8_t>(i)});
// 	}
// 	std::vector<RGB> output_gradient {};

// 	leka::ColorKit::createGradientRGB255(gradient_colors, output_gradient);
// 	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
// }

// TEST(ColorKitCreate255RGB, vectorGreenToBlue)
// {
// 	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {0, 255, 0}}, {255, {0, 0, 255}}});

// 	std::vector<RGB> expected_colors {};
// 	for (int i(0); i <= 255; ++i) {
// 		expected_colors.push_back(
// 			RGB {static_cast<uint8_t>(0), static_cast<uint8_t>(255 - i), static_cast<uint8_t>(i)});
// 	}
// 	std::vector<RGB> output_gradient {};
// 	leka::ColorKit::createGradientRGB255(gradient_colors, output_gradient);
// 	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
// }

// TEST(ColorKitCreate255RGB, vectorGreenToRed)
// {
// 	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {0, 255, 0}}, {255, {255, 0, 0}}});

// 	std::vector<RGB> expected_colors {};
// 	for (int i(0); i <= 255; ++i) {
// 		expected_colors.push_back(
// 			RGB {static_cast<uint8_t>(i), static_cast<uint8_t>(255 - i), static_cast<uint8_t>(0)});
// 	}
// 	std::vector<RGB> output_gradient {};
// 	leka::ColorKit::createGradientRGB255(gradient_colors, output_gradient);
// 	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
// }

// TEST(ColorKitCreate255RGB, vectorRedToBlue)
// {
// 	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {255, 0, 0}}, {255, {0, 0, 255}}});

// 	std::vector<RGB> expected_colors {};
// 	for (int i(0); i <= 255; ++i) {
// 		expected_colors.push_back(
// 			RGB {static_cast<uint8_t>(255 - i), static_cast<uint8_t>(0), static_cast<uint8_t>(i)});
// 	}
// 	std::vector<RGB> output_gradient {};
// 	leka::ColorKit::createGradientRGB255(gradient_colors, output_gradient);
// 	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
// }

// TEST(ColorKitCreate255RGB, vectorBlueToGreen)
// {
// 	auto gradient_colors = std::to_array<GradientSpotColor>({{0, {0, 0, 255}}, {255, {0, 255, 0}}});

// 	std::vector<RGB> expected_colors {};
// 	for (int i(0); i <= 255; ++i) {
// 		expected_colors.push_back(
// 			RGB {static_cast<uint8_t>(0), static_cast<uint8_t>(i), static_cast<uint8_t>(255 - i)});
// 	}
// 	std::vector<RGB> output_gradient {};
// 	leka::ColorKit::createGradientRGB255(gradient_colors, output_gradient);
// 	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), output_gradient.begin()));
// }
// TEST(ColorKitConversionTest, convertRandomColor)
// {
// 	auto expected_color = RGB {123, 15, 177};
// 	auto oklab_color	= ColorKit::internal::rgb2oklab(expected_color);
// 	auto rgb_color		= ColorKit::internal::oklab2rgb(oklab_color);

// 	EXPECT_EQ(rgb_color, expected_color);
// }

// TEST(ColorKitConversionTest, clampAroundZero)
// {
// 	EXPECT_EQ(ColorKit::internal::clamp(0.F), 0);
// 	EXPECT_EQ(ColorKit::internal::clamp(-0.F), 0);
// 	EXPECT_EQ(ColorKit::internal::clamp(0.001F), 0);
// 	EXPECT_EQ(ColorKit::internal::clamp(-0.001F), 0);
// }

// TEST(ColorKitConversionTest, clampNegative)
// {
// 	EXPECT_EQ(ColorKit::internal::clamp(-0.1F), 0);
// 	EXPECT_EQ(ColorKit::internal::clamp(-1.F), 0);
// 	EXPECT_EQ(ColorKit::internal::clamp(-10000.F), 0);
// }

// TEST(ColorKitConversionTest, clampInBound)
// {
// 	EXPECT_EQ(ColorKit::internal::clamp(50.F / 255.F), 50);
// 	EXPECT_EQ(ColorKit::internal::clamp(150.F / 255.F), 150);
// 	EXPECT_EQ(ColorKit::internal::clamp(200.F / 255.F), 200);
// }

// TEST(ColorKitConversionTest, clampAroundOne)
// {
// 	EXPECT_EQ(ColorKit::internal::clamp(1.F), 255);
// 	EXPECT_EQ(ColorKit::internal::clamp(0.999F), 255);
// 	EXPECT_EQ(ColorKit::internal::clamp(1.001F), 255);
// 	EXPECT_EQ(ColorKit::internal::clamp(6.001F), 255);
// }
