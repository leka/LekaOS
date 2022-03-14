// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

namespace leka::FileSystemKit::config {

const std::filesystem::path hysteresis = "/fs/conf/bootloader_hysteresis.conf";
const std::filesystem::path load	   = "/fs/conf/bootloader_load.conf";

}	// namespace leka::FileSystemKit::config
