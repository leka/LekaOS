// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/FirmwareVersion.h"
#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceUpdate : public interface::BLEService
{
  public:
	BLEServiceUpdate() : interface::BLEService(service::firmware_update::uuid, _characteristic_table) {};

	auto getApplyUpdateValue() const -> bool { return apply_update_value; }

	auto getVersion() const -> FirmwareVersion { return version; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == apply_update_characteristic.getValueHandle()) {
			apply_update_value = static_cast<bool>(params.data[0]);
		}
		if (params.handle == version_major_characteristic.getValueHandle()) {
			version.major = params.data[0];
		}
		if (params.handle == version_minor_characteristic.getValueHandle()) {
			version.minor = params.data[0];
		}
		if (params.handle == version_revision_characteristic.getValueHandle()) {
			version.revision = static_cast<uint16_t>(params.data[0] << 8 | params.data[1]);
		}
	};

  private:
	bool apply_update_value {false};
	WriteOnlyGattCharacteristic<bool> apply_update_characteristic {
		service::firmware_update::characteristic::apply_update, &apply_update_value};

	FirmwareVersion version {};

	WriteOnlyGattCharacteristic<uint8_t> version_major_characteristic {
		service::firmware_update::characteristic::version_major, &version.major};

	WriteOnlyGattCharacteristic<uint8_t> version_minor_characteristic {
		service::firmware_update::characteristic::version_minor, &version.minor};

	WriteOnlyGattCharacteristic<uint16_t> version_revision_characteristic {
		service::firmware_update::characteristic::version_revision, &version.revision};

	std::array<GattCharacteristic *, 4> _characteristic_table {
		&apply_update_characteristic, &version_major_characteristic, &version_minor_characteristic,
		&version_revision_characteristic};
};

}	// namespace leka
