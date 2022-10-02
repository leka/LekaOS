#include "BLEKit.h"

using namespace leka;

void BLEKit::setServices(std::span<interface::BLEService *> const &services)
{
	_core_gatt_server.setServices(services);
}

void BLEKit::init()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_core_gap.setEventHandler();
	_core_gatt_server.setEventHandler();

	_ble.onEventsToProcess({this, &BLEKit::processEvents});

	_core_gap.setDefaultAdvertising();

	_ble.init(&_core_gap, &CoreGap::onInitializationComplete);

	_event_queue.dispatch_forever();
}

void BLEKit::processEvents(BLE::OnEventsToProcessCallbackContext *context)
{
	_event_queue.callMbedCallback(mbed::callback(&context->ble, &BLE::processEvents));
}

void BLEKit::setAdvertisingData(const AdvertisingData &advertising_data)
{
	_advertising_data = advertising_data;

	_core_gap.setAdvertising(_advertising_data);
}

auto BLEKit::getAdvertisingData() const -> AdvertisingData
{
	return _advertising_data;
}

void BLEKit::onConnectionCallback(const std::function<void()> &callback)
{
	_core_gap.onConnectionCallback(callback);
}

void BLEKit::onDisconnectionCallback(const std::function<void()> &callback)
{
	_core_gap.onDisconnectionCallback(callback);
}
