// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"

namespace leka::utils::math {

auto computeLinearCoefficients(Point p1, Point p2) -> LinearCoefficients
{
	auto coeffs = LinearCoefficients {0, 0};

	if (p2.x == p1.x) {
		// TODO: add log.error("p1 and p2 have the same x value")
		return coeffs;
	}

	coeffs.slope	   = (p2.y - p1.y) / (p2.x - p1.x);
	coeffs.y_intercept = p1.y - coeffs.slope * p1.x;

	return coeffs;
}

}	// namespace leka::utils::math
