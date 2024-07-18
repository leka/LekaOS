// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "CoreWifi.h"

using namespace leka;

auto CoreWifi::set_credentials(const char *ssid, const char *password, nsapi_security_t security) -> int
{
	return _module.set_credentials(ssid, password, security);
}

auto CoreWifi::set_channel(uint8_t channel) -> nsapi_error_t
{
	return _module.set_channel(channel);
}

auto CoreWifi::scan(WiFiAccessPoint *res, nsapi_size_t count) -> nsapi_size_or_error_t
{
	auto mode = ESP8266Interface::scan_mode {ESP8266Interface::SCANMODE_ACTIVE};
	return _module.scan(res, count, mode);
}

auto CoreWifi::connect(const Network &network) -> bool
{
	if (auto ret = _module.connect(network.ssid, network.password, network.security); ret != NSAPI_ERROR_OK) {
		return false;
	}

	return true;
}

auto CoreWifi::connect(const char *ssid, const char *password, nsapi_security_t security,
					   uint8_t channel) -> nsapi_error_t
{
	return _module.connect(ssid, password, security, channel);
}

auto CoreWifi::connect() -> nsapi_error_t
{
	return _module.connect();
}

auto CoreWifi::disconnect() -> nsapi_error_t
{
	return _module.disconnect();
}

auto CoreWifi::get_rssi() -> int8_t
{
	return _module.get_rssi();
}

auto CoreWifi::get_stack() -> NetworkStack *
{
	return _module.get_stack();
}

// LCOV_EXCL_STOP
