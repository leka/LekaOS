// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <functional>
#include <span>
#include <tuple>

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceConfig : public interface::BLEService
{
  public:
	static constexpr uint8_t kMaxRobotNameSize {18};

	BLEServiceConfig() : interface::BLEService(service::config::uuid, _characteristic_table) {}

	void setRobotName(std::span<uint8_t> robot_name)
	{
		std::copy(std::begin(robot_name), std::begin(robot_name) + std::size(_robot_name), _robot_name.begin());

		auto data = std::make_tuple(_robot_name_characteristic.getValueHandle(), _robot_name);
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == _robot_name_characteristic.getValueHandle() && params.len <= _robot_name.size()) {
			_robot_name.fill('\0');
			std::copy(params.data, params.data + params.len, _robot_name.begin() + params.offset);
			if (_on_robot_name_updated != nullptr) {
				_on_robot_name_updated(_robot_name);
			}
		}
	}

	void onRobotNameUpdated(const std::function<void(const std::array<uint8_t, kMaxRobotNameSize> &)> &callback)
	{
		_on_robot_name_updated = callback;
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	std::array<uint8_t, kMaxRobotNameSize> _robot_name {};
	ReadWriteArrayGattCharacteristic<uint8_t, kMaxRobotNameSize> _robot_name_characteristic {
		service::config::characteristic::robot_name, _robot_name.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};
	std::function<void(const std::array<uint8_t, kMaxRobotNameSize> &)> _on_robot_name_updated {};

	std::array<GattCharacteristic *, 1> _characteristic_table {&_robot_name_characteristic};
};

}	// namespace leka
