// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>

#include "CoreGattServerEventHandler.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreGattServerEventHandler::setServices(std::span<interface::BLEService *> const &services)
{
	_services = services;
}

void CoreGattServerEventHandler::onDataWritten(const GattWriteCallbackParams &params)
{
	auto on_data_received = [&params](interface::BLEService *service) {
		auto characteristics_count = service->getCharacteristicCount();

		for (uint8_t index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataReceived(params);
			}
		}
	};

	std::for_each(_services.begin(), _services.end(), on_data_received);
}

void CoreGattServerEventHandler::onDataRead(const GattReadCallbackParams &params)
{
	auto on_data_requested = [&params](interface::BLEService *service) {
		auto characteristics_count = service->getCharacteristicCount();

		for (uint8_t index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataRequested(params);
			}
		}
	};

	std::for_each(_services.begin(), _services.end(), on_data_requested);
}

void CoreGattServerEventHandler::onMTUNegotiated(const std::function<void(uint16_t)> &callback)
{
	_on_mtu_negotiated_callback = callback;
}

void CoreGattServerEventHandler::onAttMtuChange(ble::connection_handle_t handle, uint16_t new_mtu)
{
	log_info("Negotiated MTU size: %i bytes", new_mtu);
	if (_on_mtu_negotiated_callback != nullptr) {
		_on_mtu_negotiated_callback(new_mtu);
	}
}
