// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <span>
#include <tuple>

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceFileExchange : public interface::BLEService
{
  public:
	BLEServiceFileExchange() : interface::BLEService(service::file_exchange::uuid, _characteristic_table) {};

	void setFileExchangeState(bool value)
	{
		set_file_exchange_state = static_cast<uint8_t>(value);

		auto data = std::make_tuple(&set_file_exchange_state_characteristic, std::span(&set_file_exchange_state, 1));
		sendData(data);
	}

	auto getFileExchangeState() const -> bool { return set_file_exchange_state != 0x00; }

	void setFileIsCleared()
	{
		clear_file = 0;

		auto data = std::make_tuple(&clear_file_characteristic, std::span(&clear_file, 1));
		sendData(data);
	}

	void setFileSHA256(std::array<uint8_t, 32> sha256)
	{
		std::copy(std::begin(sha256), std::begin(sha256) + std::size(sha256), file_sha256.begin());

		auto data = std::make_tuple(&file_sha256_characteristic, file_sha256);
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == set_file_exchange_state_characteristic.getValueHandle()) {
			set_file_exchange_state = params.data[0];
			if (_on_set_file_exchange_state_change) {
				_on_set_file_exchange_state_change(static_cast<bool>(set_file_exchange_state));
			}
		}
		if (params.handle == file_path_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				file_path.fill('\0');
			}
			std::copy(params.data, params.data + params.len, file_path.begin() + params.offset);
			if (_on_file_path_callback != nullptr) {
				auto on_file_data_callback_buffer = std::span {file_path.data(), params.len};
				_on_file_path_callback(on_file_data_callback_buffer);
			}
		}
		if (params.handle == clear_file_characteristic.getValueHandle()) {
			clear_file = params.data[0];
			if (_on_clear_file_requested_callback && clear_file != 0x00) {
				_on_clear_file_requested_callback();
			}
		}
		if (params.handle == file_reception_buffer_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				file_reception_buffer.fill('\0');
			}
			std::copy(params.data, params.data + params.len, file_reception_buffer.begin() + params.offset);
			if (_on_file_data_callback != nullptr) {
				auto on_file_data_callback_buffer = std::span {file_reception_buffer.data(), params.len};
				_on_file_data_callback(on_file_data_callback_buffer);
			}
		}
	};

	void onSetFileExchangeState(const std::function<void(bool)> &callback)
	{
		_on_set_file_exchange_state_change = callback;
	}

	void onFilePathReceived(const std::function<void(std::span<const char>)> &callback)
	{
		_on_file_path_callback = callback;
	}

	void onClearFileRequested(const std::function<void()> &callback) { _on_clear_file_requested_callback = callback; }

	void onFileDataReceived(const std::function<void(std::span<const uint8_t>)> &callback)
	{
		_on_file_data_callback = callback;
	}

	void onFileSHA256Requested(const std::function<void(std::span<const char>)> &callback)
	{
		_on_file_sha256_callback = callback;
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		if (params.handle == file_sha256_characteristic.getValueHandle() && _on_file_sha256_callback != nullptr) {
			_on_file_sha256_callback(file_path);
		}
	}

  private:
	uint8_t set_file_exchange_state {0x00};
	ReadWriteGattCharacteristic<uint8_t> set_file_exchange_state_characteristic {
		service::file_exchange::characteristic::set_state, &set_file_exchange_state,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};
	std::function<void(bool)> _on_set_file_exchange_state_change {};

	std::array<char, 256> file_path {};
	WriteOnlyArrayGattCharacteristic<char, 256> file_path_characteristic {
		service::file_exchange::characteristic::file_path, file_path.begin()};

	uint8_t clear_file {0x00};
	ReadWriteGattCharacteristic<uint8_t> clear_file_characteristic {
		service::file_exchange::characteristic::clear_file, &clear_file,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};
	std::function<void()> _on_clear_file_requested_callback {};

	std::array<uint8_t, 128> file_reception_buffer {};
	WriteOnlyArrayGattCharacteristic<uint8_t, 128> file_reception_buffer_characteristic {
		service::file_exchange::characteristic::file_reception_buffer, file_reception_buffer.begin()};

	std::array<uint8_t, 32> file_sha256 {};
	ReadOnlyArrayGattCharacteristic<uint8_t, 32> file_sha256_characteristic {
		service::file_exchange::characteristic::file_sha256, file_sha256.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::function<void(std::span<const uint8_t>)> _on_file_data_callback {};
	std::function<void(std::span<const char>)> _on_file_path_callback {};
	std::function<void(std::span<const char>)> _on_file_sha256_callback {};

	std::array<GattCharacteristic *, 5> _characteristic_table {
		&set_file_exchange_state_characteristic, &file_path_characteristic, &clear_file_characteristic,
		&file_reception_buffer_characteristic, &file_sha256_characteristic};
};

}	// namespace leka
