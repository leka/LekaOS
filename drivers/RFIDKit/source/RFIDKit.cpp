
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"
#include <cstdint>

namespace leka {
auto RFIDKit::init() -> bool
{
	return _rfid_reader.init();
}

auto RFIDKit::getTagData(std::array<uint8_t, 16> &tag_data) -> bool
{
	if (!_rfid_reader.setup()) {
		return false;
	}

	sendREQA();
	if (!receiveATQA()) {
		return false;
	}

	sendReadRegister8();
	if (!receiveTagData()) {
		return false;
	}

	getData(tag_data);
	return true;
}

void RFIDKit::sendREQA()
{
	std::array<uint8_t, 2> array {};

	commandToArray(command_requestA, array);

	_rfid_reader.send(array);
}

void RFIDKit::sendReadRegister8()
{
	std::array<uint8_t, 3> array {};

	commandToArray(command_read_register_8, array);

	_rfid_reader.send(array);
}

auto RFIDKit::receiveATQA() -> bool
{
	std::array<uint8_t, 2> ATQA_answer {};

	_rfid_reader.receive(ATQA_answer);

	return ATQA_answer == interface::RFID::ISO14443::ATQA_answer ? true : false;
}

auto RFIDKit::receiveTagData() -> bool
{
	_rfid_reader.receive(_tag_data);

	std::array<uint8_t, 2> received_crc = {_tag_data.data()[16], _tag_data.data()[17]};

	return received_crc == computeCRC(_tag_data.data(), 16) ? true : false;
}

auto RFIDKit::computeCRC(uint8_t const *data, size_t size) const -> std::array<uint8_t, 2>
{
	uint32_t wCrc = 0x6363;

	do {
		std::byte bt;
		bt	 = static_cast<std::byte>(*data++);
		bt	 = (bt ^ static_cast<std::byte>(wCrc & 0x00FF));
		bt	 = (bt ^ (bt << 4));
		wCrc = (wCrc >> 8) ^ (static_cast<uint32_t>(bt) << 8) ^ (static_cast<uint32_t>(bt) << 3) ^
			   (static_cast<uint32_t>(bt) >> 4);
	} while (--size);

	std::array<uint8_t, 2> pbtCrc = {static_cast<uint8_t>(wCrc & 0xFF), static_cast<uint8_t>((wCrc >> 8) & 0xFF)};
	return pbtCrc;
}

void RFIDKit::getData(std::array<uint8_t, 16> &data)
{
	std::copy(_tag_data.begin(), _tag_data.begin() + data.size(), data.begin());
}

}	// namespace leka
