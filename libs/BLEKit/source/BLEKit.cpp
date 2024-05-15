// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEKit.h"

#include "platform/Callback.h"

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
	_event_queue.call(mbed::Callback {&context->ble, &BLE::processEvents});
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

auto BLEKit::isConnected() const -> bool
{
	return _core_gap.isConnected();
}

void BLEKit::onMTUNegotiated(const std::function<void(uint16_t)> &callback)
{
	_core_gatt_server.onMTUNegotiated(callback);
}
