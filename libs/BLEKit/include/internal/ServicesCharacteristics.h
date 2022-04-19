// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::service {

namespace device_information {

	inline constexpr uint16_t uuid = GattService::UUID_DEVICE_INFORMATION_SERVICE;

	namespace characteristic {
		inline constexpr uint16_t manufacturer_name = GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR;
		inline constexpr uint16_t model_number		= GattCharacteristic::UUID_MODEL_NUMBER_STRING_CHAR;
		inline constexpr uint16_t serial_number		= GattCharacteristic::UUID_SERIAL_NUMBER_STRING_CHAR;
	}	// namespace characteristic

}	// namespace device_information

namespace battery {

	inline constexpr uint16_t uuid = GattService::UUID_BATTERY_SERVICE;

	namespace characteristic {
		inline constexpr uint16_t level = GattCharacteristic::UUID_BATTERY_LEVEL_CHAR;
	}	// namespace characteristic

}	// namespace battery

namespace monitoring {

	inline constexpr uint16_t uuid = 0x7779;

	namespace characteristic {
		inline constexpr uint16_t charging_status	 = 0x6783;
		inline constexpr uint16_t screensaver_enable = 0x8369;
		inline constexpr uint16_t soft_reboot		 = 0x8382;
		inline constexpr uint16_t hard_reboot		 = 0x7282;
	}	// namespace characteristic

}	// namespace monitoring

namespace receive_file {

	inline constexpr uint16_t uuid = 0x8270;

	namespace characteristic {
		inline constexpr uint16_t file_path				= 0x7080;
		inline constexpr uint16_t file_reception_buffer = 0x8283;
	}	// namespace characteristic

}	// namespace receive_file

namespace firmware_update {

	inline constexpr uint16_t uuid = 0x7085;

	namespace characteristic {
		inline constexpr uint16_t apply_update = 0x6585;

		inline constexpr uint16_t version_major	   = 0x7765;
		inline constexpr uint16_t version_minor	   = 0x7773;
		inline constexpr uint16_t version_revision = 0x8269;
	}	// namespace characteristic

}	// namespace firmware_update

namespace commands {

	inline constexpr uint16_t uuid = 0xDFB0;

	namespace characteristic {
		inline constexpr uint16_t tx = 0xDFB1;
	}	// namespace characteristic

}	// namespace commands

}	// namespace leka::service
