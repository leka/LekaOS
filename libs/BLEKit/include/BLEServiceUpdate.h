// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceUpdate : public interface::BLEService
{
  public:
	BLEServiceUpdate() : interface::BLEService(service::firmware_update::uuid, _characteristic_table) {};

	auto getApplyUpdateValue() const -> bool { return apply_update_value; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == apply_update_characteristic.getValueHandle()) {
			apply_update_value = static_cast<bool>(params.data[0]);
		}
	};

  private:
	bool apply_update_value {false};
	WriteOnlyGattCharacteristic<bool> apply_update_characteristic {
		service::firmware_update::characteristic::apply_update, &apply_update_value};

	std::array<GattCharacteristic *, 1> _characteristic_table {&apply_update_characteristic};
};

}	// namespace leka
