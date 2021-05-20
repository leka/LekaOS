// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {

LKCoreRFID::LKCoreRFID(interface::BufferedSerial &interface) : _interface(interface) {}

auto LKCoreRFID::setRFIDTag(RFIDTag const &expected_values) -> void
{
	_rfid_tag = expected_values;
}

auto LKCoreRFID::getRFIDTag() const -> RFIDTag
{
	return _rfid_tag;
}

template <size_t N>
auto LKCoreRFID::send(const std::array<uint8_t, N> &command) -> void
{
	_interface.write(command.data(), N);
}

auto LKCoreRFID::setProtocol() -> void
{
	const uint8_t cmd_CR95HF = 0x02;
	const uint8_t length	 = 0x02;
	const uint8_t cmd_tag	 = 0x02;
	const uint8_t flags		 = 0x00;

	std::array<uint8_t, 4> command = {cmd_CR95HF, length, cmd_tag, flags};

	send(command);
}

auto LKCoreRFID::setGain() -> void
{
	const uint8_t cmd_CR95HF			= 0x09;
	const uint8_t length				= 0x04;
	const uint8_t ARC_B_register_adress = 0x68;
	const uint8_t increment_index		= 0x01;
	const uint8_t gain_value_index		= 0x01;
	const uint8_t gain_value			= 0xD1;

	std::array<uint8_t, 6> command = {cmd_CR95HF,	   length,			 ARC_B_register_adress,
									  increment_index, gain_value_index, gain_value};

	send(command);
}

auto LKCoreRFID::receiveSetupAnswer() -> bool
{
	std::array<uint8_t, 2> buffer;

	_interface.read(buffer.data(), buffer.size());

	return checkSensorSetup(buffer.data());
}

auto LKCoreRFID::checkSensorSetup(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> CR95HF_setup_completed = {0x00, 0x00};

	if (std::array<uint8_t, 2> CR95HF_answer = {buffer[0], buffer[1]};
		CR95HF_answer[0] == CR95HF_setup_completed[0] && CR95HF_answer[1] == CR95HF_setup_completed[1]) {
		return true;
	}

	return false;
}

auto LKCoreRFID::sendREQA() -> void
{
	const uint8_t cmd_CR95HF = 0x04;
	const uint8_t length	 = 0x02;
	const uint8_t cmd_tag	 = 0x26;
	const uint8_t flags		 = 0x07;

	std::array<uint8_t, 4> command = {cmd_CR95HF, length, cmd_tag, flags};

	send(command);
}

auto LKCoreRFID::checkATQA(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> ATQA_NTAG213_value = {0x44, 0x00};

	if (std::array<uint8_t, 2> ATQA_tag_answer = {buffer[2], buffer[3]};
		(ATQA_tag_answer[0] == ATQA_NTAG213_value[0]) && (ATQA_tag_answer[1] == ATQA_NTAG213_value[1])) {
		return true;
	}

	return false;
}

auto LKCoreRFID::receiveATQA() -> bool
{
	std::array<uint8_t, 7> buffer;

	_interface.read(buffer.data(), buffer.size());

	return checkATQA(buffer.data());
}

auto LKCoreRFID::sendCL1() -> void
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x03;
	const std::array<uint8_t, 2> cmd_tag = {0x93, 0x20};
	const uint8_t flags					 = 0x08;

	std::array<uint8_t, 5> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], flags};

	send(command);
}

auto LKCoreRFID::sendCL2() -> void
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x03;
	const std::array<uint8_t, 2> cmd_tag = {0x95, 0x20};
	const uint8_t flags					 = 0x08;

	std::array<uint8_t, 5> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], flags};

	send(command);
}

auto LKCoreRFID::setUID2(uint8_t *buffer) -> void
{
	std::copy_n(buffer + 2, 4, _rfid_tag.UID.begin() + 4);
	_rfid_tag.crc_UID[1] = buffer[6];
}

auto LKCoreRFID::receiveUID2() -> void
{
	std::array<uint8_t, 10> buffer;

	_interface.read(buffer.data(), buffer.size());

	setUID2(buffer.data());
}

auto LKCoreRFID::setUID1(uint8_t *buffer) -> void
{
	std::copy_n(buffer + 2, 4, _rfid_tag.UID.begin());
	_rfid_tag.crc_UID[0] = buffer[6];
}

auto LKCoreRFID::receiveUID1() -> void
{
	std::array<uint8_t, 10> buffer;

	_interface.read(buffer.data(), buffer.size());

	setUID1(buffer.data());
}

auto LKCoreRFID::sendUID1() -> void
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x08;
	const std::array<uint8_t, 2> cmd_tag = {0x93, 0x70};
	const std::array<uint8_t, 4> uid	 = {_rfid_tag.UID[0], _rfid_tag.UID[1], _rfid_tag.UID[2], _rfid_tag.UID[3]};
	const uint8_t uid_crc				 = _rfid_tag.crc_UID[0];
	const uint8_t flags					 = 0x28;

	std::array<uint8_t, 10> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], uid[0],
									   uid[1],	   uid[2], uid[3],	   uid_crc,	   flags};

	send(command);
}

auto LKCoreRFID::sendUID2() -> void
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x08;
	const std::array<uint8_t, 2> cmd_tag = {0x95, 0x70};
	const std::array<uint8_t, 4> uid	 = {_rfid_tag.UID[4], _rfid_tag.UID[5], _rfid_tag.UID[6], _rfid_tag.UID[7]};
	const uint8_t uid_crc				 = _rfid_tag.crc_UID[1];
	const uint8_t flags					 = 0x28;

	std::array<uint8_t, 10> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], uid[0],
									   uid[1],	   uid[2], uid[3],	   uid_crc,	   flags};

	send(command);
}

auto LKCoreRFID::setSAK1(uint8_t *buffer) -> void
{
	std::copy_n(buffer + 2, 2, _rfid_tag.SAK.begin());
}

auto LKCoreRFID::receiveSAK1() -> void
{
	std::array<uint8_t, 8> buffer;

	_interface.read(buffer.data(), buffer.size());

	setSAK1(buffer.data());
}

auto LKCoreRFID::setSAK2(uint8_t *buffer) -> void
{
	std::copy_n(buffer + 2, 2, _rfid_tag.SAK.begin() + 2);
}

auto LKCoreRFID::receiveSAK2() -> void
{
	std::array<uint8_t, 8> buffer;

	_interface.read(buffer.data(), buffer.size());

	setSAK2(buffer.data());
}
}	// namespace leka
