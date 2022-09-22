// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CastUtils.h"
#include "interface/drivers/FirmwareVersion.h"
#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceDeviceInformation : public interface::BLEService
{
  public:
	BLEServiceDeviceInformation() : interface::BLEService(service::device_information::uuid, _characteristic_table) {}

	void setSerialNumber(std::span<uint8_t> const &value) const
	{
		std::copy(std::begin(value), std::begin(value) + std::size(_serial_number), _serial_number.begin());

		auto data = std::make_tuple(_serial_number_characteristic.getValueHandle(), _serial_number);
		sendData(data);
	}

	void setOSVersion(const FirmwareVersion &version) const
	{
		_os_version.fill('\0');
		auto version_cstr = version.asStdArray();

		std::copy(std::begin(version_cstr), std::begin(version_cstr) + std::size(_os_version), _os_version.begin());

		auto data = std::make_tuple(_os_version_characteristic.getValueHandle(), _os_version);
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final
	{
		// do nothing
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	inline static auto _manufacturer = utils::cast::from_c_string_to_uint8_t_array("APF France handicap");

	GattCharacteristic _manufacturer_name_characteristic {
		service::device_information::characteristic::manufacturer_name, _manufacturer.data(), _manufacturer.size(),
		_manufacturer.size(), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ};

	inline static auto _model = utils::cast::from_c_string_to_uint8_t_array("Leka");

	GattCharacteristic _model_number_characteristic {service::device_information::characteristic::model_number,
													 _model.data(), _model.size(), _model.size(),
													 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ};

	inline static auto _serial_number = utils::cast::from_c_string_to_uint8_t_array("LK-2202-0123456789ABCDEFGHIJKLMN");

	GattCharacteristic _serial_number_characteristic {
		service::device_information::characteristic::serial_number, _serial_number.data(), _serial_number.size(),
		_serial_number.size(), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ};

	inline static auto _os_version = utils::cast::from_c_string_to_uint8_t_array("000.000.00000");

	GattCharacteristic _os_version_characteristic {service::device_information::characteristic::os_version,
												   _os_version.data(), _os_version.size(), _os_version.size(),
												   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ};

	std::array<GattCharacteristic *, 4> _characteristic_table {
		&_manufacturer_name_characteristic, &_model_number_characteristic, &_serial_number_characteristic,
		&_os_version_characteristic};
};

}	// namespace leka
