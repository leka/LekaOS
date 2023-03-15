// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <filesystem>

namespace leka {

template <std::size_t SIZE = 1>
struct Config {
  public:
	explicit Config(const std::filesystem::path &path, std::array<uint8_t, SIZE> default_value = {})
		: _path(_default_parent_path / path), kDefaultValue(default_value)
	{
		// nothing to do
	}

	[[nodiscard]] auto path() const -> std::filesystem::path { return _path; }
	[[nodiscard]] auto default_value() const -> std::array<uint8_t, SIZE> { return kDefaultValue; }

  private:
	const std::filesystem::path _default_parent_path = "/fs/etc";
	const std::filesystem::path _path;
	const std::array<uint8_t, SIZE> kDefaultValue {};
};

}	// namespace leka
