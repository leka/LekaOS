// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"

namespace leka::utils {

LinearFunctionCoefficients computeLinearCoefficients(float x1, float x2, float y1, float y2)
{
	auto coefficients = LinearFunctionCoefficients {0, 0};

	if (x2 - x1 == 0) {
		return coefficients;
	}

	coefficients.slope		 = (y2 - y1) / (x2 - x1);
	coefficients.y_intercept = y1 - coefficients.slope * x1;

	return coefficients;
}

std::tuple<float, float> computeLinearCoefficientsTuple(float x1, float x2, float y1, float y2)
{
	auto tuple = std::make_tuple(0., 0.);

	if (x2 - x1 == 0) {
		return tuple;
	}

	std::get<0>(tuple) = (y2 - y1) / (x2 - x1);
	std::get<1>(tuple) = y1 - std::get<0>(tuple) * x1;

	return tuple;
}

}	// namespace leka::utils
