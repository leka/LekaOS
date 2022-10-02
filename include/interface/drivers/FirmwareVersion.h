// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <cstdio>
#include <span>

namespace leka {

struct FirmwareVersion {
	uint8_t major;
	uint8_t minor;
	uint16_t revision;	 // ? Use uint16_t instead of uint8_t for compatibility w/ MCUBoot Image format, see
						 // ? https://docs.mcuboot.com/design.html

	[[nodiscard]] auto asStdArray() const -> std::span<const char>
	{
		std::snprintf(_formatted_output.data(), std::size(_formatted_output), _format_string, major, minor, revision);
		return std::span(_formatted_output);
	}

	const char *_format_string					   = "%u.%u.%u";
	mutable std::array<char, 14> _formatted_output = {};
};

}	// namespace leka
