// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

namespace leka {

struct Config {
  public:
	explicit Config(const std::filesystem::path &path, uint8_t default_value = 0xFF)
		: _path(_default_parent_path / path), _default_value(default_value)
	{
		// nothing to do
	}

	[[nodiscard]] auto path() const -> std::filesystem::path { return _path; }
	[[nodiscard]] auto default_value() const -> uint8_t { return _default_value; }

  private:
	const std::filesystem::path _default_parent_path = "/fs/etc";
	const std::filesystem::path _path;
	const uint8_t _default_value;
};

}	// namespace leka
