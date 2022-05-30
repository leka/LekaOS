// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

#include "LogKit.h"

namespace leka {
void RFIDKit::init()
{
	static auto getTagDataCallback = [this]() { this->runStateMachine(); };

	_rfid_reader.registerTagAvailableCallback(getTagDataCallback);
	_rfid_reader.init();
	_rfid_reader.setTagDetectionMode();
}

void RFIDKit::runStateMachine()
{
	switch (_state) {
		case state::WAITING_FOR_TAG: {
			if (_rfid_reader.isTagDetected()) {
				_rfid_reader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
				_state = state::TAG_COMMUNICATION_PROTOCOL_SET;
			} else {
				_rfid_reader.setTagDetectionMode();
			}

		} break;

		case state::TAG_COMMUNICATION_PROTOCOL_SET: {
			sendREQA();
			_state = state::WAIT_FOR_ATQA_RESPONSE;

		} break;

		case state::WAIT_FOR_ATQA_RESPONSE: {
			if (receiveATQA()) {
				sendReadRegister0();
				_state = state::TAG_IDENTIFIED;
			} else {
				_rfid_reader.setTagDetectionMode();
				_state = state::WAITING_FOR_TAG;
			}
		} break;

		case state::TAG_IDENTIFIED: {
			if (receiveReadTagData()) {
				sendReadRegister4();
				_state = state::TAG_AVAILABLE;
			} else {
				_rfid_reader.setTagDetectionMode();
				_state = state::WAITING_FOR_TAG;
			}
		} break;
		case state::TAG_AVAILABLE: {
			if (receiveReadTagData() && isTagSignatureValid()) {
				_card = MagicCard {_tag};
				log_info("Card available: 0x%04X", _card.getId());
				_on_tag_available_callback(_card);
			}
			_rfid_reader.setTagDetectionMode();
			_state = state::WAITING_FOR_TAG;

		} break;
		default: {
			break;
		}
	}
}

void RFIDKit::onTagActivated(std::function<void(MagicCard &_card)> callback)
{
	_on_tag_available_callback = callback;
}

void RFIDKit::sendREQA()
{
	_rfid_reader.sendCommandToTag(command_requestA.getArray());
}

void RFIDKit::sendReadRegister0()
{
	_rfid_reader.sendCommandToTag(command_read_register_0.getArray());
}

void RFIDKit::sendReadRegister4()
{
	_rfid_reader.sendCommandToTag(command_read_register_4.getArray());
}

auto RFIDKit::isTagSignatureValid() -> bool
{
	return (_tag.data[0] == 0x4C && _tag.data[1] == 0x45 && _tag.data[2] == 0x4B && _tag.data[3] == 0x41);
}

auto RFIDKit::receiveATQA() -> bool
{
	std::array<uint8_t, 2> ATQA_answer {};
	std::span<uint8_t> span = {ATQA_answer};

	_rfid_reader.receiveDataFromTag(span);

	return (span[0] == interface::RFIDReader::ISO14443::ATQA_answer[0] &&
			span[1] == interface::RFIDReader::ISO14443::ATQA_answer[1]);
}

auto RFIDKit::receiveReadTagData() -> bool
{
	std::span<uint8_t> span = {_tag_data};
	_rfid_reader.receiveDataFromTag(span);

	for (size_t i = 0; i < span.size(); ++i) {
		_tag.data[i] = span.data()[i];
	}

	std::array<uint8_t, 2> received_crc = {span[16], span[17]};

	return received_crc == computeCRC(span.data());
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
