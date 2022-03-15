// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::service {

namespace device_information {

	constexpr uint16_t uuid = GattService::UUID_DEVICE_INFORMATION_SERVICE;

	namespace characteristic {
		constexpr uint16_t manufacturer_name = GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR;
		constexpr uint16_t model_number		 = GattCharacteristic::UUID_MODEL_NUMBER_STRING_CHAR;
		constexpr uint16_t serial_number	 = GattCharacteristic::UUID_SERIAL_NUMBER_STRING_CHAR;
	}	// namespace characteristic

}	// namespace device_information

namespace battery {

	constexpr uint16_t uuid = GattService::UUID_BATTERY_SERVICE;

	namespace characteristic {
		constexpr uint16_t level = GattCharacteristic::UUID_BATTERY_LEVEL_CHAR;
	}	// namespace characteristic

}	// namespace battery

namespace monitoring {

	constexpr uint16_t uuid = 0x7779;

	namespace characteristic {
		constexpr uint16_t charging_status = 0x6783;
	}	// namespace characteristic

}	// namespace monitoring

namespace firmware_update {

	constexpr uint16_t uuid = 0x7085;

	namespace characteristic {
		constexpr uint16_t apply_update = 0x6585;

		constexpr uint16_t version_major	= 0x7765;
		constexpr uint16_t version_minor	= 0x7773;
		constexpr uint16_t version_revision = 0x8269;
	}	// namespace characteristic

}	// namespace firmware_update

}	// namespace leka::service
