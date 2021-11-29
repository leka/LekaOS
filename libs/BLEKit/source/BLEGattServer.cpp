// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEGattServer.h"

using namespace leka;
using namespace std::chrono_literals;

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

	event_queue.call_every(1s, this, &BLEGattServer::updateData);
}

void BLEGattServer::onDataWritten(const GattWriteCallbackParams &params)
{
	for (auto &service: _services) {
		auto characteristics_count = service->getCharacteristicCount();
		for (int index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataWritten(params);
			}
		}
	}
}

void BLEGattServer::updateData()
{
	updateServiceFunction update_server_data = [&](GattAttribute::Handle_t characteristic_updated, const uint8_t *data,
												   uint16_t n_data_bytes) {
		_server.write(characteristic_updated, data, n_data_bytes);
	};

	for (auto &service: _services) {
		auto characteristics_count = service->getCharacteristicCount();
		for (int index = 0; index < characteristics_count; index++) {
			service->updateData(update_server_data);
		}
	}
}
