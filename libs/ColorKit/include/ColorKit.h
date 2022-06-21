// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "GradientSpotColor.h"
#include "RGB.h"
#include "internal/conversion.h"
#include "internal/interpolation.h"

namespace leka::ColorKit {

auto colorGradient(const RGB &color1, const RGB &color2, const float &position) -> RGB;

auto createGradientRGB255(std::span<GradientSpotColor> gradient_colors, const RGB &color_start, const RGB &color_end,
						  std::span<RGB> output_gradient) -> void;

}	// namespace leka::ColorKit
