// Leka - LekaOS
// Copyright 2022 APF France handicap
// Based on work by @gurki - https://github.com/gurki/vivid
// SPDX-License-Identifier: Apache-2.0

#include "internal/conversion.h"
#include <cmath>

#include "RGB.h"

using namespace leka;

auto ColorKit::internal::clamp(const float &value) -> uint8_t
{
	if (value > 1.F) {
		return 255;
	}
	if (value < 0.F) {
		return 0;
	}

	const auto rounded = static_cast<uint8_t>(std::round(value * 255.F));

	return rounded;
}

auto ColorKit::internal::oklab2lrgb(const oklab_t &oklab) -> lrgb_t
{
	const auto l_ = oklab.x + 0.3963377774F * oklab.y + 0.2158037573F * oklab.z;
	const auto m_ = oklab.x - 0.1055613458F * oklab.y - 0.0638541728F * oklab.z;
	const auto s_ = oklab.x - 0.0894841775F * oklab.y - 1.2914855480F * oklab.z;

	const auto l = l_ * l_ * l_;
	const auto m = m_ * m_ * m_;
	const auto s = s_ * s_ * s_;

	const auto lrgb = lrgb_t {
		+4.0767416621F * l - 3.3077115913F * m + 0.2309699292F * s,
		-1.2684380046F * l + 2.6097574011F * m - 0.3413193965F * s,
		-0.0041960863F * l - 0.7034186147F * m + 1.7076147010F * s,
	};

	return lrgb;
}

auto ColorKit::internal::lrgb2oklab(const lrgb_t &lrgb) -> oklab_t
{
	const auto l = 0.4122214708F * lrgb.x + 0.5363325363F * lrgb.y + 0.0514459929F * lrgb.z;
	const auto m = 0.2119034982F * lrgb.x + 0.6806995451F * lrgb.y + 0.1073969566F * lrgb.z;
	const auto s = 0.0883024619F * lrgb.x + 0.2817188376F * lrgb.y + 0.6299787005F * lrgb.z;

	const auto l_ = std::cbrtf(l);
	const auto m_ = std::cbrtf(m);
	const auto s_ = std::cbrtf(s);

	const auto oklab = oklab_t {
		0.2104542553F * l_ + 0.7936177850F * m_ - 0.0040720468F * s_,
		1.9779984951F * l_ - 2.4285922050F * m_ + 0.4505937099F * s_,
		0.0259040371F * l_ + 0.7827717662F * m_ - 0.8086757660F * s_,
	};

	return oklab;
}

auto ColorKit::internal::rgb2oklab(const RGB &rgb) -> oklab_t
{
	const auto lrgb	 = lrgb_t {static_cast<float>(rgb.red) / 255.F, static_cast<float>(rgb.green) / 255.F,
							   static_cast<float>(rgb.blue) / 255.F};
	const auto oklab = lrgb2oklab(lrgb);

	return oklab;
}

auto ColorKit::internal::oklab2rgb(const oklab_t &oklab) -> RGB
{
	const auto lrgb = oklab2lrgb(oklab);
	const auto rgb	= RGB {clamp(lrgb.r), clamp(lrgb.g), clamp(lrgb.b)};

	return rgb;
}
