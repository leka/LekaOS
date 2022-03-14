// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <filesystem>
#include <span>

namespace leka::FileSystemKit::config {

const std::uint8_t max_buffer_size = 16;

auto get(const char *path, uint8_t pos = 0) -> size_t;
auto set(const char *path, uint8_t data, uint8_t pos = 0) -> void;

auto get(std::filesystem::path path, uint8_t pos = 0) -> size_t;
auto set(std::filesystem::path path, uint8_t data, uint8_t pos = 0) -> void;

}	// namespace leka::FileSystemKit::config
