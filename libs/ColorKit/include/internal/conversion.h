// Leka - LekaOS
// Copyright 2022 APF France handicap
// Based on work by @gurki - https://github.com/gurki/vivid
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "RGB.h"
#include "types.h"

namespace leka::ColorKit::internal {

auto clamp(const float &value) -> uint8_t;

auto oklab2lrgb(const oklab_t &oklab) -> lrgb_t;

auto lrgb2oklab(const lrgb_t &lrgb) -> oklab_t;

auto rgb2oklab(const RGB &rgb) -> oklab_t;

auto oklab2rgb(const oklab_t &oklab) -> RGB;

}	// namespace leka::ColorKit::internal
