// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <cstdint>
#include <numbers>
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

[[nodiscard]] auto random8(uint8_t min, uint8_t max) -> uint8_t;

template <typename OutputType>
auto sinPeriod(uint8_t period, uint8_t step, uint8_t offset, OutputType out_min, OutputType out_max) -> float
{
	auto pi			   = std::numbers::pi_v<float>;
	auto mapped_offset = math::map(offset, uint8_t {0}, uint8_t {255}, 0.F, 2.F * pi);
	auto sinus = static_cast<float>(std::sin(2 * pi * (1.F / static_cast<float>(period)) * step + mapped_offset));
	return (out_max - out_min) * (sinus + 1.F) / 2.F;
}

}	// namespace leka::utils::math
