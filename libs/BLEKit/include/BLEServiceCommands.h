// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <variant>

#include "CommandKit.h"
#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceBlunoCompatibility : public interface::BLEService
{
  public:
	BLEServiceBlunoCompatibility() : interface::BLEService(service::commands::uuid, _characteristic_table) {};

	void onDataReceived(const data_received_handle_t &params) final {
		// nothing to do
	};

  private:
	std::array<uint8_t, 128> _buffer {};
	ReadOnlyGattCharacteristic<uint8_t> bluno_characteristic {service::commands::characteristic::tx, _buffer.begin()};

	std::array<GattCharacteristic *, 1> _characteristic_table {&bluno_characteristic};
};

class BLEServiceCommands : public interface::BLEService
{
  public:
	BLEServiceCommands(CommandKit &kit)
		: _cmdkit(kit), interface::BLEService(service::commands::uuid, _characteristic_table) {};

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == tx_characteristic.getValueHandle()) {
			std::copy(params.data, params.data + params.len, _buffer.begin());
			// _cmdkit.push(std::span {_buffer.data(), params.len})
			_cmdkit.push(std::span {_buffer.data(), params.len});
		}
	};

  private:
	std::array<uint8_t, 128> _buffer {};
	WriteOnlyArrayGattCharacteristic<uint8_t, 128> tx_characteristic {
		service::commands::characteristic::tx, _buffer.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE};

	std::array<GattCharacteristic *, 1> _characteristic_table {&tx_characteristic};
	CommandKit &_cmdkit;
};

}	// namespace leka
