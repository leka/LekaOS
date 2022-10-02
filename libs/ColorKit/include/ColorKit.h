// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "RGB.h"
#include "internal/conversion.h"
#include "internal/interpolation.h"

namespace leka::ColorKit {

auto colorGradient(const RGB &color1, const RGB &color2, const float &position) -> RGB;

}	// namespace leka::ColorKit
