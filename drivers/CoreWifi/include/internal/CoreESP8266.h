// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "ESP8266Interface.h"

namespace leka {

class CoreESP8266 : public ESP8266Interface
{
	using ESP8266Interface::ESP8266Interface;

  public:
	auto get_stack() -> NetworkStack * override { return this; }
};

}	// namespace leka

// LCOV_EXCL_STOP
