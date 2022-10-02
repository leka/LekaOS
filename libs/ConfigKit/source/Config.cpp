// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ConfigKit.h"
#include "FileManagerKit.h"

using namespace leka;

auto ConfigKit::read(Config const &config) const -> uint8_t
{
	FileManagerKit::File file {config.path(), "r"};

	if (!file.is_open()) {
		return config.default_value();
	}

	auto data = std::array<uint8_t, 1> {};
	file.read(data);

	return data.front();
}

auto ConfigKit::write(Config const &config, uint8_t data) const -> bool
{
	FileManagerKit::File file {config.path(), "w+"};

	if (!file.is_open()) {
		return false;
	}

	auto output = std::to_array<uint8_t>({data});
	file.write(output);

	return true;
}
