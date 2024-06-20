// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <span>
#include <tuple>

#include "MagicCard.h"
#include "MemoryUtils.h"
#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceMagicCard : public interface::BLEService
{
  public:
	BLEServiceMagicCard() : interface::BLEService(service::magic_card::uuid, _characteristic_table) {};

	void setMagicCard(MagicCard const &card)
	{
		sendRawData(card.getId(), card.getLanguage());
		sendID(card.getId());
		sendLanguage(card.getLanguage());
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
	void sendRawData(uint16_t id, MagicCard::Language language)
	{
		raw_data[0] = utils::memory::getHighByte(id);
		raw_data[1] = utils::memory::getLowByte(id);
		raw_data[2] = static_cast<uint8_t>(language);

		auto data = std::make_tuple(raw_data_characteristic.getValueHandle(), raw_data);
		sendData(data);
	}

	void sendID(uint16_t id)
	{
		_id[0] = utils::memory::getHighByte(id);
		_id[1] = utils::memory::getLowByte(id);

		auto data = std::make_tuple(_id_characteristic.getValueHandle(), _id);
		sendData(data);
	}

	void sendLanguage(MagicCard::Language language)
	{
		_language = static_cast<uint8_t>(language);

		auto data = std::make_tuple(_language_characteristic.getValueHandle(), std::span(&_language, 1));
		sendData(data);
	}

	std::array<uint8_t, 2> _id {};
	ReadOnlyArrayGattCharacteristic<uint8_t, 2> _id_characteristic {
		service::magic_card::characteristic::id, _id.begin(), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	uint8_t _language {0x00};
	ReadOnlyGattCharacteristic<uint8_t> _language_characteristic {
		service::magic_card::characteristic::language, &_language, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::array<uint8_t, 3> raw_data {};
	ReadOnlyArrayGattCharacteristic<uint8_t, 3> raw_data_characteristic {
		service::magic_card::characteristic::raw_data, raw_data.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::array<GattCharacteristic *, 3> _characteristic_table {&raw_data_characteristic, &_id_characteristic,
															   &_language_characteristic};
};

}	// namespace leka
