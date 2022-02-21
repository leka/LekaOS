// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGattServerEventHandler.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreGattServerEventHandler::setServices(std::span<interface::BLEService *> const &services)
{
	_services = services;
}

void CoreGattServerEventHandler::onDataWritten(const GattWriteCallbackParams &params)
{
	auto applyOnDataWritten = [&params](interface::BLEService *service) {
		auto characteristics_count = service->getCharacteristicCount();
		for (uint8_t index = 0; index < characteristics_count; index++) {
			if (params.handle == service->getCharacteristic(index)->getValueHandle()) {
				service->onDataWritten(params);
			}
		}
	};

	std::for_each(_services.begin(), _services.end(), applyOnDataWritten);
}
