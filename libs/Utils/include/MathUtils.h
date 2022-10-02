// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

namespace leka::utils::math {

template <typename OutputType, typename IntputType>
auto map(IntputType x, IntputType in_min, IntputType in_max, OutputType out_min, OutputType out_max) -> OutputType
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

struct Point {
	float x {0};
	float y {0};
};

struct LinearCoefficients {
	float slope {0};
	float y_intercept {0};
};

auto computeLinearCoefficients(Point p1, Point p2) -> LinearCoefficients;

auto checksum8(std::span<uint8_t> data) -> uint8_t;

}	// namespace leka::utils::math
