// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "FileManagerKit.h"
#include "LogKit.h"
#include "ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

auto FileManagerKit::create_directory(const std::filesystem::path &path) -> bool
{
	return std::filesystem::create_directory(path);
}

auto FileManagerKit::remove(const std::filesystem::path &path) -> bool
{
	return std::filesystem::remove(path);
}

void FileManagerKit::list_contents(const std::filesystem::path &path)
{
	for (const auto &content: std::filesystem::directory_iterator(path.c_str())) {
		log_info(" %s ", content.path().c_str());
		rtos::ThisThread::sleep_for(50ms);
	}
}
