// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

#include "Config.h"

namespace leka {

class ConfigKit
{
  public:
	explicit ConfigKit() = default;
	[[nodiscard]] auto read(Config const &config) const -> uint8_t;
	[[nodiscard]] auto write(Config const &config, uint8_t data) const -> bool;
};

}	// namespace leka
