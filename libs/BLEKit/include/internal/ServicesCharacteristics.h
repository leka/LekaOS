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

	inline constexpr UUID::LongUUIDBytes_t uuid = {0x00, 0x00, 0x77, 0x79};

	namespace characteristic {
		inline constexpr UUID::LongUUIDBytes_t charging_status	  = {0x00, 0x00, 0x67, 0x83};
		inline constexpr UUID::LongUUIDBytes_t screensaver_enable = {"ENA Screensaver"};
		inline constexpr UUID::LongUUIDBytes_t soft_reboot		  = {0x00, 0x00, 0x83, 0x82};
		inline constexpr UUID::LongUUIDBytes_t hard_reboot		  = {0x00, 0x00, 0x72, 0x82};
	}	// namespace characteristic

}	// namespace monitoring

namespace config {

	inline constexpr uint16_t uuid = 0x6770;

	namespace characteristic {
		inline constexpr uint16_t robot_name = 0x8278;
	}	// namespace characteristic

}	// namespace config

namespace magic_card {
	inline constexpr UUID::LongUUIDBytes_t uuid = {"Magic Card"};

	namespace characteristic {
		inline constexpr UUID::LongUUIDBytes_t id		= {"ID"};
		inline constexpr UUID::LongUUIDBytes_t language = {"Language"};
	}	// namespace characteristic

}	// namespace magic_card

namespace file_exchange {

	inline constexpr UUID::LongUUIDBytes_t uuid = {0x00, 0x00, 0x82, 0x70};

	namespace characteristic {
		inline constexpr UUID::LongUUIDBytes_t set_state			 = {0x00, 0x00, 0x83, 0x83};
		inline constexpr UUID::LongUUIDBytes_t file_path			 = {0x00, 0x00, 0x70, 0x80};
		inline constexpr UUID::LongUUIDBytes_t clear_file			 = {0x00, 0x00, 0x67, 0x70};
		inline constexpr UUID::LongUUIDBytes_t file_reception_buffer = {0x00, 0x00, 0x82, 0x83};
		inline constexpr UUID::LongUUIDBytes_t file_sha256			 = {0x00, 0x00, 0x70, 0x83};
	}	// namespace characteristic

}	// namespace file_exchange

namespace firmware_update {

	inline constexpr UUID::LongUUIDBytes_t uuid = {0x00, 0x00, 0x70, 0x85};

	namespace characteristic {
		inline constexpr UUID::LongUUIDBytes_t request_update		 = {0x00, 0x00, 0x82, 0x85};
		inline constexpr UUID::LongUUIDBytes_t request_factory_reset = {0x00, 0x00, 0x82, 0x70};

		inline constexpr UUID::LongUUIDBytes_t version_major	= {0x00, 0x00, 0x77, 0x65};
		inline constexpr UUID::LongUUIDBytes_t version_minor	= {0x00, 0x00, 0x77, 0x73};
		inline constexpr UUID::LongUUIDBytes_t version_revision = {0x00, 0x00, 0x82, 0x69};
	}	// namespace characteristic

}	// namespace firmware_update

namespace commands {

	inline constexpr uint16_t uuid = 0xDFB0;

	namespace characteristic {
		inline constexpr uint16_t tx = 0xDFB1;
	}	// namespace characteristic

}	// namespace commands

}	// namespace leka::service
