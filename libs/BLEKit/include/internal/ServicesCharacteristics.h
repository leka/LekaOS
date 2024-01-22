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
		inline constexpr uint16_t os_version		= GattCharacteristic::UUID_FIRMWARE_REVISION_STRING_CHAR;
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
		inline constexpr uint16_t charging_status			  = 0x6783;
		inline constexpr uint16_t screensaver_enable		  = 0x8369;
		inline constexpr uint16_t soft_reboot				  = 0x8382;
		inline constexpr uint16_t hard_reboot				  = 0x7282;
		inline constexpr UUID::LongUUIDBytes_t negotiated_mtu = {"NEGOTIATED_MTU"};
		inline constexpr UUID::LongUUIDBytes_t temperature	  = {"TEMPERATURE"};
	}	// namespace characteristic

}	// namespace monitoring

namespace config {

	inline constexpr uint16_t uuid = 0x6770;

	namespace characteristic {
		inline constexpr uint16_t robot_name = 0x8278;
	}	// namespace characteristic

}	// namespace config

namespace magic_card {
	inline constexpr UUID::LongUUIDBytes_t uuid = {"MAGIC_CARD"};

	namespace characteristic {
		inline constexpr UUID::LongUUIDBytes_t raw_data = {"ID+LANGUAGE"};
	}	// namespace characteristic

}	// namespace magic_card

namespace file_exchange {

	inline constexpr uint16_t uuid = 0x8270;

	namespace characteristic {
		inline constexpr uint16_t set_state				= 0x8383;
		inline constexpr uint16_t file_path				= 0x7080;
		inline constexpr uint16_t clear_file			= 0x6770;
		inline constexpr uint16_t file_reception_buffer = 0x8283;
		inline constexpr uint16_t file_sha256			= 0x7083;
	}	// namespace characteristic

}	// namespace file_exchange

namespace firmware_update {

	inline constexpr uint16_t uuid = 0x7085;

	namespace characteristic {
		inline constexpr uint16_t request_update		= 0x8285;
		inline constexpr uint16_t request_factory_reset = 0x8270;

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
