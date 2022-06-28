// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::utils::memory {

constexpr auto getLowByte(uint16_t value) noexcept -> uint8_t
{
	return static_cast<uint8_t>(value);
}

constexpr auto getHighByte(uint16_t value) noexcept -> uint8_t
{
	return static_cast<uint8_t>(value >> 8);
}

struct TwoBytes {
	uint8_t high {};
	uint8_t low {};
};

constexpr auto combineBytes(TwoBytes bytes) noexcept -> uint16_t
{
	auto both = uint16_t {};

	both = bytes.high;
	both = static_cast<uint16_t>(both << 8);
	both |= bytes.low;

	return both;
}

constexpr auto getByteAndMask(uint8_t value, uint8_t mask) noexcept -> uint8_t
{
	return static_cast<uint8_t>(mask & value);
}

constexpr auto getByteOrMask(uint8_t value, uint8_t mask) noexcept -> uint8_t
{
	return static_cast<uint8_t>(mask | value);
}

constexpr auto combineBytesParts(uint8_t first_value, uint8_t second_value, uint8_t mask) noexcept -> uint8_t
{
	return static_cast<uint8_t>((mask & first_value) | (~mask & second_value));
}

}	// namespace leka::utils::memory
