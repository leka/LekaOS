// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "FileManagerKit.h"
#include "LogKit.h"

using namespace leka;

auto FileManagerKit::create_directory(const std::filesystem::path &path) -> bool
{
	return std::filesystem::create_directory(path);
}

auto FileManagerKit::remove(const std::filesystem::path &path) -> bool
{
	return std::filesystem::remove(path);
}
