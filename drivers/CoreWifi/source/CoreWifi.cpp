// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreWifi.h"

using namespace leka;

CoreWifi::CoreWifi(WiFiInterface &interface, PinName reset_pin, PinName enable_pin)
	: _interface {interface}, _nreset {reset_pin, 0}, _enable {enable_pin, 1}
{
	_nreset = 1;
}

bool CoreWifi::connect()
{
	if (_network_name.empty() || _network_password.empty()) {
		return false;
	}

	auto ret = _interface.connect(_network_name.data(), _network_password.data(), NSAPI_SECURITY_WPA_WPA2);
	return ret == NSAPI_ERROR_OK;
}

void CoreWifi::disconnect()
{
	_interface.disconnect();
}
