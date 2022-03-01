// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::service {

namespace battery {

	constexpr uint16_t uuid = GattService::UUID_BATTERY_SERVICE;

	namespace characteristic {
		constexpr uint16_t level = GattCharacteristic::UUID_BATTERY_LEVEL_CHAR;
	}

}	// namespace battery

}	// namespace leka::service
