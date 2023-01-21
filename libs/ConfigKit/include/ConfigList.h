// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

#include "ConfigKit.h"

namespace leka::config::bootloader {

inline const auto battery_level_hysteresis = Config {"bootloader_battery_level_hysteresis", {42}};

}	// namespace leka::config::bootloader
