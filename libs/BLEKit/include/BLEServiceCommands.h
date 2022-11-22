// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <span>

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceCommands : public interface::BLEService
{
  public:
	explicit BLEServiceCommands() : interface::BLEService(service::commands::uuid, _characteristic_table) {}

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == tx_characteristic.getValueHandle()) {
			std::copy(params.data, params.data + params.len, _buffer.begin());

			if (_on_commands_received) {
				_on_commands_received(std::span {_buffer.data(), params.len});
			}
		}
	}

	void onCommandsReceived(const std::function<void(std::span<uint8_t>)> &callback)
	{
		_on_commands_received = callback;
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	std::array<uint8_t, 128> _buffer {};
	WriteOnlyArrayGattCharacteristic<uint8_t, 128> tx_characteristic {
		service::commands::characteristic::tx, _buffer.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE};

	std::function<void(std::span<uint8_t>)> _on_commands_received;

	std::array<GattCharacteristic *, 1> _characteristic_table {&tx_characteristic};
};

}	// namespace leka
