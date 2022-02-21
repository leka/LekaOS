// Leka - LekaOS
// Copyright 2022 APF France handicap
// Based on work by @gurki - https://github.com/gurki/vivid
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "types.h"

namespace leka::ColorKit::internal {

inline auto lerp(const oklab_t &col1, const oklab_t &col2, float t) -> oklab_t
{
	return static_cast<oklab_t>(col1 + t * (col2 - col1));
};

}	// namespace leka::ColorKit::internal
