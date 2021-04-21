// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"

namespace leka::utils {

std::pair<float, float> computeLinearCoefficientsPair(float x1, float x2, float y1, float y2)
{
	// auto pair = LinearFunctionCoefficientsPair;
	auto pair = std::make_pair(0., 0.);

	if (x2 - x1 == 0) {
		return pair;
	}

	pair.first	= (y2 - y1) / (x2 - x1);
	pair.second = y1 - pair.first * x1;

	return pair;
}

std::tuple<float, float> computeLinearCoefficientsTuple(float x1, float x2, float y1, float y2)
{
	// auto pair = LinearFunctionCoefficientsPair;
	auto tuple = std::make_tuple(0., 0.);

	if (x2 - x1 == 0) {
		return tuple;
	}

	std::get<0>(tuple) = (y2 - y1) / (x2 - x1);
	std::get<1>(tuple) = y1 - std::get<0>(tuple) * x1;

	return tuple;
}

}	// namespace leka::utils
