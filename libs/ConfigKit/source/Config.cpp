// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <span>

#include "ConfigKit.h"
#include "LogKit.h"

using namespace leka;

auto ConfigKit::read(Config const &config) -> uint8_t
{
	FileManagerKit::File file {config.path(), "r"};
	if (!file.is_open()) {
		return config.default_value();
	}
	auto input = std::array<uint8_t, 1> {};
	file.read(input);
	auto data = input.front();
	return data;
}

auto ConfigKit::write(Config const &config, uint8_t data) -> bool
{
	FileManagerKit::File file {config.path(), "r+"};
	if (!file.is_open()) {
		return false;
	}
	auto output = std::array<uint8_t, 1> {data};
	file.write(output);
	return true;
}
