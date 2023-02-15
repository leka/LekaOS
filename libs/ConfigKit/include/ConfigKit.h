// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

#include "Config.h"
#include "FileManagerKit.h"

namespace leka {

class ConfigKit
{
  public:
	explicit ConfigKit() = default;
	template <std::size_t SIZE = 1>
	[[nodiscard]] auto read(Config<SIZE> const &config) const
	{
		if (FileManagerKit::file_is_missing(config.path())) {
			if constexpr (SIZE == 1) {
				return config.default_value()[0];
			} else {
				return config.default_value();
			}
		}

		auto file = FileManagerKit::File {config.path(), "r"};

		auto data = std::array<uint8_t, SIZE> {};
		file.read(data);

		if constexpr (SIZE == 1) {
			return data[0];
		} else {
			return data;
		}
	}

	template <std::size_t SIZE = 1>
	[[nodiscard]] auto write(Config<SIZE> const &config, std::array<uint8_t, SIZE> data) const -> bool
	{
		if (FileManagerKit::File file {config.path(), "w+"}; file.is_open()) {
			file.write(data);

			return true;
		}

		return false;
	}
};

}	// namespace leka
