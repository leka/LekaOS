// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LKUTILS_H_
#define _LEKA_OS_LIB_LKUTILS_H_

namespace leka::utils {

template <typename OutputType, typename IntputType>
OutputType map(IntputType x, IntputType in_min, IntputType in_max, OutputType out_min, OutputType out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

namespace math {

	struct Point {
		float x {0.f};
		float y {0.f};
	};

	struct LinearCoefficients {
		float slope {0.f};
		float y_intercept {0.f};
	};

	auto computeLinearCoefficients(Point p1, Point p2) -> LinearCoefficients;

}	// namespace math

}	// namespace leka::utils

#endif	 // _LEKA_OS_LIB_LKUTILS_H_
