// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceRobot : public interface::BLEService
{
  public:
	BLEServiceRobot() : interface::BLEService(service::robot::uuid, _characteristic_table) {};

	auto getSleepValue() const -> bool { return sleep_value; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == _sleep_characteristic.getValueHandle()) {
			sleep_value = static_cast<bool>(params.data[0]);
		}
	};

  private:
	bool sleep_value {false};
	WriteOnlyGattCharacteristic<bool> _sleep_characteristic {service::robot::characteristic::sleep, &sleep_value};

	std::array<GattCharacteristic *, 1> _characteristic_table {&_sleep_characteristic};
};

}	// namespace leka
