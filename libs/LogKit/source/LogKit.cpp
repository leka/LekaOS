// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"

using namespace leka;

rtos::Mutex logger::mutex = rtos::Mutex();

const std::unordered_map<logger::level, std::string_view> logger::level_lut = {
	{logger::level::debug, "[DBUG]"},
	{logger::level::info, "[INFO]"},
	{logger::level::error, "[ERR ]"},
};
