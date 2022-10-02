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
	BLEServiceUpdate() : interface::BLEService(service::firmware_update::uuid, _characteristic_table) {}

	auto getVersion() const -> FirmwareVersion { return version; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == apply_update_characteristic.getValueHandle()) {
			must_apply_update = static_cast<bool>(params.data[0]);
			if (must_apply_update && _on_update_requested_callback != nullptr) {
				_on_update_requested_callback();
			}
		}
		if (params.handle == factory_reset_characteristic.getValueHandle()) {
			is_factory_reset_requested = static_cast<bool>(params.data[0]);
			if (_on_factory_reset_notification_callback != nullptr) {
				_on_factory_reset_notification_callback(is_factory_reset_requested);
			}
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
	}

	void onUpdateRequested(const std::function<void()> &callback) { _on_update_requested_callback = callback; }

	void onFactoryResetNotification(const std::function<void(bool)> &callback)
	{
		_on_factory_reset_notification_callback = callback;
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	bool must_apply_update {false};
	WriteOnlyGattCharacteristic<bool> apply_update_characteristic {
		service::firmware_update::characteristic::request_update, &must_apply_update};
	std::function<void()> _on_update_requested_callback {};

	bool is_factory_reset_requested {false};
	WriteOnlyGattCharacteristic<bool> factory_reset_characteristic {
		service::firmware_update::characteristic::request_factory_reset, &is_factory_reset_requested};
	std::function<void(bool)> _on_factory_reset_notification_callback {};

	FirmwareVersion version {};

	WriteOnlyGattCharacteristic<uint8_t> version_major_characteristic {
		service::firmware_update::characteristic::version_major, &version.major};

	WriteOnlyGattCharacteristic<uint8_t> version_minor_characteristic {
		service::firmware_update::characteristic::version_minor, &version.minor};

	WriteOnlyGattCharacteristic<uint16_t> version_revision_characteristic {
		service::firmware_update::characteristic::version_revision, &version.revision};

	std::array<GattCharacteristic *, 5> _characteristic_table {
		&apply_update_characteristic, &factory_reset_characteristic, &version_major_characteristic,
		&version_minor_characteristic, &version_revision_characteristic};
};

}	// namespace leka
