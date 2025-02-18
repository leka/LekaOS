// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "CoreESP8266.h"
#include "netsocket/WiFiInterface.h"

namespace leka {

class CoreWifi : public WiFiInterface
{
  public:
	CoreWifi(CoreESP8266 &wifi_module, mbed::interface::DigitalOut &enable) : _module(wifi_module), _enable(enable) {}

	struct Network {
		const char *ssid {};
		const char *password {};
		const nsapi_security_t security = NSAPI_SECURITY_WPA_WPA2;
	};

	auto set_credentials(const char *ssid, const char *password, nsapi_security_t security = NSAPI_SECURITY_NONE)
		-> int final;
	auto set_channel(uint8_t channel) -> nsapi_error_t final;

	auto scan(WiFiAccessPoint *res, nsapi_size_t count) -> nsapi_size_or_error_t final;

	auto connect(const Network &network) -> bool;
	auto connect(const char *ssid, const char *password, nsapi_security_t security = NSAPI_SECURITY_NONE,
				 uint8_t channel = 0) -> nsapi_error_t final;
	auto connect() -> nsapi_error_t final;
	auto disconnect() -> nsapi_error_t final;

	auto get_rssi() -> int8_t final;
	auto get_stack() -> NetworkStack * final;

  private:
	CoreESP8266 &_module;
	mbed::interface::DigitalOut &_enable;
};

}	// namespace leka

// LCOV_EXCL_STOP
