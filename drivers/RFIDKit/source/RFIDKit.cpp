
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"
#include <cstdint>

namespace leka {
void RFIDKit::setInterrupt(void func())
{
	interface::BufferedSerial &serial = _rfid_reader.getSerial();
	serial.sigio(func);
}

void RFIDKit::setReaderForTagDetection()
{
	_rfid_reader.enableTagDetection();
}

auto RFIDKit::getTagData(std::array<uint8_t, 16> &tag_data) -> bool
{
	if (!_rfid_reader.receiveCallback()) {
		return false;
	}

	if (!_rfid_reader.setup()) {
		return false;
	}

	sendREQA();
	if (!receiveATQA()) {
		return false;
	}

	sendReadRegister8();
	if (!receiveReadTagData()) {
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
	lstd::span<uint8_t> span = {ATQA_answer};

	_rfid_reader.receiveTagData(&span);

	return (span[0] == interface::RFID::ISO14443::ATQA_answer[0] &&
			span[1] == interface::RFID::ISO14443::ATQA_answer[1])
			   ? true
			   : false;
}

auto RFIDKit::receiveReadTagData() -> bool
{
	lstd::span<uint8_t> span = {_tag_data};
	_rfid_reader.receiveTagData(&span);

	for (size_t i = 0; i < span.size(); ++i) {
		_tag_data[i] = span.data()[i];
	}

	std::array<uint8_t, 2> received_crc = {span.data()[16], span.data()[17]};

	return received_crc == computeCRC(_tag_data.data()) ? true : false;
}

auto RFIDKit::computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>
{
	uint32_t wCrc = 0x6363;
	size_t size	  = 16;

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
