// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "ColorKit.h"
#include <cmath>

#include "Utils.h"

namespace leka {

auto ColorKit::colorGradient(const RGB &color1, const RGB &color2, const float &position) -> RGB
{
	return oklab2rgb(lerp(ColorKit::internal::rgb2oklab(color1), ColorKit::internal::rgb2oklab(color2), position));
}

auto ColorKit::createGradientRGB255(std::span<GradientSpotColor> gradient_colors, const RGB &color_start,
									const RGB &color_end, std::span<RGB> output_gradient) -> void
{
	GradientSpotColor first_input;
	GradientSpotColor last_input;
	if (!gradient_colors.empty()) {
		first_input = gradient_colors.front();
		last_input	= gradient_colors.back();

	} else {
		first_input.color	 = color_start;
		first_input.position = 0;
		last_input.color	 = color_start;
		last_input.position	 = 0;
	}
	uint8_t size = gradient_colors.size();

	auto fill_colors_before_first_input_colors = [output_gradient, color_start, first_input] {
		for (auto i = 0; i < first_input.position; ++i) {
			float index_color =
				leka::utils::math::map(static_cast<uint8_t>(i), uint8_t {0}, uint8_t {first_input.position}, 0.F, 1.F);
			output_gradient[i] = ColorKit::colorGradient(color_start, first_input.color, index_color);
		}
	};
	auto fill_colors_between_gradient_colors = [output_gradient, gradient_colors, size] {
		for (auto i = 0; i < size - 1; ++i) {
			for (auto j = gradient_colors[i].position; j < gradient_colors[i + 1].position; ++j) {
				float index_color = leka::utils::math::map(j, uint8_t {gradient_colors[i].position},
														   uint8_t {gradient_colors[i + 1].position}, 0.F, 1.F);
				output_gradient[j] =
					ColorKit::colorGradient(gradient_colors[i].color, gradient_colors[i + 1].color, index_color);
			}
		}
	};
	auto fill_colors_after_last_input_colors = [output_gradient, color_end, last_input] {
		for (auto i = last_input.position; i < 255; ++i) {
			float index_color  = leka::utils::math::map(i, uint8_t {last_input.position}, uint8_t {255}, 0.F, 1.F);
			output_gradient[i] = ColorKit::colorGradient(last_input.color, color_end, index_color);
		}
	};
	if (first_input.position != 0) {
		fill_colors_before_first_input_colors();
	}
	fill_colors_between_gradient_colors();
	if (last_input.position != 255) {
		fill_colors_after_last_input_colors();
	}
}

}	// namespace leka
