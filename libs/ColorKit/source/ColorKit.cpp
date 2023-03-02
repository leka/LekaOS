// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ColorKit.h"
#include <cmath>

#include "internal/conversion.h"
#include "internal/interpolation.h"

namespace leka {

auto ColorKit::colorGradient(const RGB &color1, const RGB &color2, const float &position) -> RGB
{
	return oklab2rgb(lerp(ColorKit::internal::rgb2oklab(color1), ColorKit::internal::rgb2oklab(color2), position));
}

}	// namespace leka
