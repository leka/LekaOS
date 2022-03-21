// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <variant>

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceCommands : public interface::BLEService
{
  public:
	BLEServiceCommands() : interface::BLEService(service::commands::uuid, _characteristic_table) {};

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == tx_characteristic.getValueHandle()) {
			std::copy(params.data, params.data + params.len, _buffer.begin() + params.offset);
		}
	};

  private:
	std::array<char, 128> _buffer {};
	WriteOnlyArrayGattCharacteristic<char, 128> tx_characteristic {service::commands::characteristic::tx,
																   _buffer.begin()};

	std::array<GattCharacteristic *, 2> _characteristic_table {&tx_characteristic};
};

}	// namespace leka
