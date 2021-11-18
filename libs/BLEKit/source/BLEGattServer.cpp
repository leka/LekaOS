// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEGattServer.h"

using namespace leka;

void BLEGattServer::addServices(lstd::span<interface::BLEService *> services)
{
	_services = std::move(services);

	for (auto &service: _services) {
		_server.addService(*service);
	}
}

void BLEGattServer::start(BLE &ble, events::EventQueue &event_queue)
{
	_server.setEventHandler(this);
}

void BLEGattServer::onDataWritten(const GattWriteCallbackParams &params)
{
	for (auto &service: _services) {
		auto characteristics_count = service->getCharacteristicCount();
		for (int index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataUpdated(params);
			}
		}
	}
}
