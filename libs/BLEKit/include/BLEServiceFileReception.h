// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceFileReception : public interface::BLEService
{
  public:
	BLEServiceFileReception() : interface::BLEService(service::receive_file::uuid, _characteristic_table) {};

	[[nodiscard]] auto getFilePath() const -> std::span<const char> { return file_path; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == file_path_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				file_path.fill('\0');
			}
			std::copy(params.data, params.data + params.len, file_path.begin() + params.offset);
		}
		if (params.handle == file_reception_buffer_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				file_reception_buffer.fill('\0');
			}
			std::copy(params.data, params.data + params.len, file_reception_buffer.begin() + params.offset);
			if (_on_file_data_callback) {
				auto on_file_data_callback_buffer = std::span {file_reception_buffer.data(), params.len};
				_on_file_data_callback(on_file_data_callback_buffer);
			}
		}
	};

	void onFileDataReceived(const std::function<void(std::span<uint8_t> &)> &callback)
	{
		_on_file_data_callback = callback;
	}

  private:
	std::array<char, 256> file_path {};
	WriteOnlyArrayGattCharacteristic<char, 256> file_path_characteristic {
		service::receive_file::characteristic::file_path, file_path.begin()};

	std::array<uint8_t, 128> file_reception_buffer {};
	WriteOnlyArrayGattCharacteristic<uint8_t, 128> file_reception_buffer_characteristic {
		service::receive_file::characteristic::file_reception_buffer, file_reception_buffer.begin()};

	std::function<void(std::span<uint8_t> &)> _on_file_data_callback {};

	std::array<GattCharacteristic *, 2> _characteristic_table {&file_path_characteristic,
															   &file_reception_buffer_characteristic};
};

}	// namespace leka
