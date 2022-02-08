// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGattServerEventHandler.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreGattServerEventHandler::registerServerWrite(updateServiceFunction const &function)
{
	update_server_data = function;
}

void CoreGattServerEventHandler::setServices(std::span<interface::BLEService *> const &services)
{
	_services = services;
}

void CoreGattServerEventHandler::onDataWritten(const GattWriteCallbackParams &params)
{
	for (auto &service: _services) {
		auto characteristics_count = service->getCharacteristicCount();
		for (uint8_t index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataWritten(params);
			}
		}
	}
}

void CoreGattServerEventHandler::updateData()
{
	for (auto &service: _services) {
		auto characteristics_count = service->getCharacteristicCount();
		for (uint8_t index = 0; index < characteristics_count; index++) {
			service->updateData(update_server_data);
		}
	}
}
