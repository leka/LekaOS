
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"
#include <cstdint>
#include <iterator>

namespace leka {
void RFIDKit::init()
{
	static auto getTagDataCallback = [this]() { this->getTagData(); };

	_rfid_reader.registerTagAvailableCallback(getTagDataCallback);
	_rfid_reader.init();
}

void RFIDKit::getTagData()
{
	switch (_state) {
		case state::TAG_DETECTED: {
			if (_rfid_reader.checkForTagDetection()) {
				_rfid_reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
				_state = state::TAG_PROTOCOL_SET;
			} else {
				_rfid_reader.setModeTagDetection();
			}

		} break;

		case state::TAG_PROTOCOL_SET: {
			sendREQA();
			_state = state::WAIT_FOR_ATQA_RESPONSE;

		} break;

		case state::WAIT_FOR_ATQA_RESPONSE: {
			if (receiveATQA()) {
				sendReadRegister();
				_state = state::TAG_IDENTIFIED;
			} else {
				_rfid_reader.setModeTagDetection();
				_state = state::TAG_DETECTED;
			}
		} break;

		case state::TAG_IDENTIFIED: {
			_rfid_reader.setModeTagDetection();
			_state = state::TAG_DETECTED;

		} break;
	}
}

void RFIDKit::sendREQA()
{
	std::array<uint8_t, 2> array {};

	commandToArray(command_requestA, array);

	_rfid_reader.sendCommandToTag(array);
}

void RFIDKit::sendReadRegister()
{
	std::array<uint8_t, 3> array {};

	commandToArray(command_read_register_8, array);

	_rfid_reader.sendCommandToTag(array);
}

void RFIDKit::sendWriteRegister(uint8_t registerToWrite, std::array<uint8_t, 4> data)
{
	std::array<uint8_t, 7> array {};

	array[0] = 0xA2;
	array[1] = registerToWrite;

	for (int i = 0; i < 4; ++i) {
		array[i + 2] = data[i];
	}
	array[6] = 0x28;

	_rfid_reader.sendCommandToTag(array);
}

void RFIDKit::receiveWriteTagData()
{
	std::array<uint8_t, 2> ATQA_answer {};
	lstd::span<uint8_t> span = {ATQA_answer};

	_rfid_reader.receiveDataFromTag(&span);
}

void RFIDKit::sendAuthentificate()
{
	std::array<uint8_t, 6> array {};

	array[0] = 0x1B;
	array[1] = 0xff;
	array[2] = 0xff;
	array[3] = 0xff;
	array[4] = 0xff;
	array[5] = 0x28;

	_rfid_reader.sendCommandToTag(array);
}

void RFIDKit::receiveAuthentificate()
{
	std::array<uint8_t, 4> authentificate_answer {};
	lstd::span<uint8_t> span = {authentificate_answer};

	_rfid_reader.receiveDataFromTag(&span);
}

auto RFIDKit::receiveATQA() -> bool
{
	std::array<uint8_t, 2> ATQA_answer {};
	lstd::span<uint8_t> span = {ATQA_answer};

	_rfid_reader.receiveDataFromTag(&span);

	return (span[0] == interface::RFID::ISO14443::ATQA_answer[0] &&
			span[1] == interface::RFID::ISO14443::ATQA_answer[1]);
}

auto RFIDKit::receiveReadTagData() -> bool
{
	lstd::span<uint8_t> span = {_tag_data};
	_rfid_reader.receiveDataFromTag(&span);

	for (size_t i = 0; i < span.size(); ++i) {
		_tag.data[i] = span.data()[i];
	}

	std::array<uint8_t, 2> received_crc = {span[16], span[17]};

	return received_crc == computeCRC(span.data()) ? true : false;
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

}	// namespace leka
