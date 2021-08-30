// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_UTILS_MEMORY_H_
#define _LEKA_OS_LIB_UTILS_MEMORY_H_

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

}	// namespace leka::utils::memory

#endif	 // _LEKA_OS_LIB_UTILS_MEMORY_H_
