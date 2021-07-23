// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_WIFI_ESP8266_H_
#define _LEKA_OS_DRIVER_CORE_WIFI_ESP8266_H_

// ? LCOV_EXCL_START - Exclude from coverage report

#include "ESP8266Interface.h"

namespace leka {

class CoreESP8266 : public ESP8266Interface
{
	using ESP8266Interface::ESP8266Interface;

  public:
	auto get_stack() -> NetworkStack * override { return this; };
};

}	// namespace leka

// ? LCOV_EXCL_STOP - Exclude from coverage report

#endif	 // _LEKA_OS_DRIVER_CORE_WIFI_ESP8266_H_
