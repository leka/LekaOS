#include "BLEUtils.h"

// #include "BLEServiceExample.h"

using namespace leka;

void BLEUtils::setDeviceName(const lstd::span<const char> name)
{
	_ble_gap.setDeviceName(name);
}

void BLEUtils::startAdvertising()
{
	// BLEServiceExample ble_service;
	// std::array<interface::BLEService *, 1> services = {&ble_service};
	// _ble_server.addServices(services);

	_ble_gap.onInit(mbed::Callback(&_ble_server, &BLEGattServer::start));

	_ble_gap.start();
}
