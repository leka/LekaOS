// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MathUtils.h"

namespace leka::utils::math {

auto computeLinearCoefficients(Point p1, Point p2) -> LinearCoefficients
{
	if (p2.x == p1.x) {
		return {0, 0};
	}

	auto coeffs		   = LinearCoefficients {0, 0};
	coeffs.slope	   = (p2.y - p1.y) / (p2.x - p1.x);
	coeffs.y_intercept = p1.y - coeffs.slope * p1.x;

	return coeffs;
}

}	// namespace leka::utils::math
