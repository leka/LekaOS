// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>

#include "FileManagerKit.h"
#include "interface/platform/File.h"

namespace leka {

struct Config {
  public:
	explicit Config(const std::filesystem::path &path, uint8_t default_value = 0xFF)
		: _path(_default_parent_path / path), _default_value(default_value)
	{
	}

	[[nodiscard]] auto path() const -> std::filesystem::path { return _path; }
	[[nodiscard]] auto default_value() const -> uint8_t { return _default_value; }

  private:
	const std::filesystem::path _default_parent_path = "/fs/conf";
	const std::filesystem::path _path;
	const uint8_t _default_value;
};

class ConfigKit
{
  public:
	explicit ConfigKit() = default;
	auto read(Config const &config) const -> uint8_t;
	auto write(Config const &config, uint8_t data) const -> bool;
};

}	// namespace leka
