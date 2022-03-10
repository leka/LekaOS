// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>

namespace leka::utils::cast {

template <std::size_t N>
constexpr auto from_c_string_to_uint8_t_array(const char (&arr)[N]) -> std::array<uint8_t, N>
{
	std::array<uint8_t, N> data;
	std::copy(std::begin(arr), std::end(arr), data.begin());
	return data;
}

}	// namespace leka::utils::cast
