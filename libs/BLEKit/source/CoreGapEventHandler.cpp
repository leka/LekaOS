// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGapEventHandler.h"

using namespace leka;
using namespace ble;

void CoreGapEventHandler::registerStartAdvertising(std::function<void()> const &function)
{
	_start_advertising = function;
}

void CoreGapEventHandler::onInitializationComplete(BLE::InitializationCompleteCallbackContext *params) const
{
	if (params->error != BLE_ERROR_NONE) {
		return;
	}

	_start_advertising();
}

void CoreGapEventHandler::onConnectionComplete(ConnectionCompleteEvent const &event)
{
	if (event.getStatus() != BLE_ERROR_NONE) {
		_start_advertising();
	}

	if (_on_connection_callback != nullptr) {
		_on_connection_callback();
	}
}

void CoreGapEventHandler::onDisconnectionComplete(DisconnectionCompleteEvent const &event)
{
	_start_advertising();

	if (_on_disconnection_callback != nullptr) {
		_on_disconnection_callback();
	}
}

void CoreGapEventHandler::onAdvertisingEnd(AdvertisingEndEvent const &event)
{
	_start_advertising();
}

void CoreGapEventHandler::onConnectionCallback(const std::function<void()> &callback)
{
	_on_connection_callback = callback;
}

void CoreGapEventHandler::onDisconnectionCallback(const std::function<void()> &callback)
{
	_on_disconnection_callback = callback;
}
