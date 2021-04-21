// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"

leka::utils::LinearFunctionCoefficients leka::utils::computeLinearCoefficients(float x1, float x2, float y1, float y2)
{
	leka::utils::LinearFunctionCoefficients coefficients {0, 0};

	if (x2 - x1 == 0) {
		return coefficients;
	}

	coefficients.slope		 = (y2 - y1) / (x2 - x1);
	coefficients.y_intercept = y1 - coefficients.slope * x1;

	return coefficients;
}
