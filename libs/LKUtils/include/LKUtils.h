// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LKUTILS_H_
#define _LEKA_OS_LIB_LKUTILS_H_

#include <tuple>

namespace leka::utils {

template <typename OutputType, typename IntputType>
OutputType map(IntputType x, IntputType in_min, IntputType in_max, OutputType out_min, OutputType out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

namespace math {

	struct LinearFunctionCoefficients {
		float slope {0};
		float y_intercept {0};
	};

	LinearFunctionCoefficients computeLinearCoefficients(float x1, float x2, float y1, float y2);
	std::tuple<float, float> computeLinearCoefficientsTuple(float x1, float x2, float y1, float y2);

}	// namespace math

}	// namespace leka::utils

#endif	 // _LEKA_OS_LIB_LKUTILS_H_
