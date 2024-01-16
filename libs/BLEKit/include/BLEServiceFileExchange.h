// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <span>
#include <tuple>

#include "CoreTimeout.h"
#include "LogKit.h"
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

		auto data = std::make_tuple(set_file_exchange_state_characteristic.getValueHandle(),
									std::span(&set_file_exchange_state, 1));
		sendData(data);
	}

	auto getFileExchangeState() const -> bool { return set_file_exchange_state != 0x00; }

	void setFileIsCleared()
	{
		clear_file = 0;

		auto data = std::make_tuple(clear_file_characteristic.getValueHandle(), std::span(&clear_file, 1));
		sendData(data);
	}

	void setFileSHA256(std::array<uint8_t, 32> sha256)
	{
		std::copy(std::begin(sha256), std::begin(sha256) + std::size(sha256), file_sha256.begin());

		auto data = std::make_tuple(file_sha256_characteristic.getValueHandle(), file_sha256);
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
			// START
			// log_info("Params:");
			// log_info("  offset: %d", params.offset);
			// log_info("  len: %d", params.len);
			// log_info("  error_code: %d", params.error_code);
			// log_info("  connHandle: %s", params.connHandle);
			// log_info("  handle: %d", params.handle);
			// log_info("  data: %s", params.data);
			// log_info("  writeOp: %d", params.writeOp);
			// log_info("  status: %d", params.status);
			/*
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:77] onDataReceived > Params:
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:78] onDataReceived >   offset: 122
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:79] onDataReceived >   len: 6
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:80] onDataReceived >   error_code: 6
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:81] onDataReceived >   connHandle:
			x..ÔÜ6eQ˘ªX_ˇsˇ;..PQï_.'˚™.mΩKΩ±~,pB±1‘î˜Nñæ.µ¸.ÄY`¥®Gbµe∑Î¶ﬂøè.4°ÈE.Í≈º∆êÇõÀÍ-FD.“ÖòÑ_.ﬂìﬂc›H
			!Vé£âÈÔ„c&“nªªé<Ca,®..ñç◊/y˝«ﬂ.§
			....'=ÁˇõVá.∫2'.Ip .·åq.>Ï◊.˘P§ˆ".f@.]ÂM÷˙f]0à’‡“.Å.ö»Ækπª{i!åÏÉ..*.ß´õ”úâL∂!Ù.∫ò¢.H˝w€ò˙Û≥œ∫.∞”..µ.¶∑a”7ßp¨
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:82] onDataReceived >   handle: 30
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:83] onDataReceived >   data: 345678..
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:84] onDataReceived >   writeOp: 6
			000:01:16.800 [INFO] [BLEServiceFileExchange.h:85] onDataReceived >   status: 122
			*/

			if (params.offset == 0) {
				file_reception_buffer.fill('\0');
			}
			std::copy(params.data, params.data + params.len, file_reception_buffer.begin() + params.offset);
			// Reset timer on buffer fill + circular queue to avoid colusion

			if (_on_file_data_callback != nullptr) {
				using namespace std::chrono_literals;
				_timeout.stop();
				buffer_len = params.offset + params.len;

				auto on_timeout = [this] {
					auto on_file_data_callback_buffer = std::span {file_reception_buffer.data(), buffer_len};
					_on_file_data_callback(on_file_data_callback_buffer);
				};
				_timeout.onTimeout(on_timeout);
				_timeout.start(1ms);
			}

			// STOP
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

	std::array<uint8_t, 256> file_reception_buffer {};
	WriteOnlyArrayGattCharacteristic<uint8_t, 256> file_reception_buffer_characteristic {
		service::file_exchange::characteristic::file_reception_buffer, file_reception_buffer.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE};

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

	CoreTimeout _timeout {};
	uint8_t buffer_len {0};
};

}	// namespace leka
