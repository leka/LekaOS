// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFID.h"

namespace leka {

CoreRFID::CoreRFID(interface::BufferedSerial &interface) : _interface(interface) {}

auto CoreRFID::setRFIDTag(std::array<uint8_t, 8> &tag_uid, std::array<uint8_t, 2> &tag_crc_uid,
						  std::array<uint8_t, 4> &tag_sak, std::array<uint8_t, 16> &tag_data) -> void
{
	my_tag.uid	   = tag_uid;
	my_tag.crc_uid = tag_crc_uid;
	my_tag.sak	   = tag_sak;
	my_tag.data	   = tag_data;
}

auto CoreRFID::getTag() const -> CoreRFID::Tag
{
	return my_tag;
}

template <size_t N>
void CoreRFID::send(const std::array<uint8_t, N> &command)
{
	_interface.write(command.data(), N);
}

void CoreRFID::setProtocol()
{
	const uint8_t cmd_CR95HF = 0x02;
	const uint8_t length	 = 0x02;
	const uint8_t cmd_tag	 = 0x02;
	const uint8_t flags		 = 0x00;

	std::array<uint8_t, 4> command = {cmd_CR95HF, length, cmd_tag, flags};

	send(command);
}

void CoreRFID::setGain()
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

auto CoreRFID::receiveSetupAnswer() -> bool
{
	std::array<uint8_t, 2> buffer;

	_interface.read(buffer.data(), buffer.size());

	return checkSensorSetup(buffer.data());
}

auto CoreRFID::checkSensorSetup(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> CR95HF_setup_completed = {0x00, 0x00};

	if (std::array<uint8_t, 2> CR95HF_answer = {buffer[0], buffer[1]};
		CR95HF_answer[0] == CR95HF_setup_completed[0] && CR95HF_answer[1] == CR95HF_setup_completed[1]) {
		return true;
	}

	return false;
}

void CoreRFID::sendREQA()
{
	const uint8_t cmd_CR95HF = 0x04;
	const uint8_t length	 = 0x02;
	const uint8_t cmd_tag	 = 0x26;
	const uint8_t flags		 = 0x07;

	std::array<uint8_t, 4> command = {cmd_CR95HF, length, cmd_tag, flags};

	send(command);
}

auto CoreRFID::checkATQA(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> ATQA_NTAG213_value = {0x44, 0x00};

	if (std::array<uint8_t, 2> ATQA_tag_answer = {buffer[2], buffer[3]};
		(ATQA_tag_answer[0] == ATQA_NTAG213_value[0]) && (ATQA_tag_answer[1] == ATQA_NTAG213_value[1])) {
		return true;
	}

	return false;
}

auto CoreRFID::receiveATQA() -> bool
{
	std::array<uint8_t, 7> buffer;

	_interface.read(buffer.data(), buffer.size());

	return checkATQA(buffer.data());
}

void CoreRFID::sendCL1()
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x03;
	const std::array<uint8_t, 2> cmd_tag = {0x93, 0x20};
	const uint8_t flags					 = 0x08;

	std::array<uint8_t, 5> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], flags};

	send(command);
}

void CoreRFID::sendCL2()
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x03;
	const std::array<uint8_t, 2> cmd_tag = {0x95, 0x20};
	const uint8_t flags					 = 0x08;

	std::array<uint8_t, 5> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], flags};

	send(command);
}

void CoreRFID::setUID2(uint8_t *buffer)
{
	std::copy_n(buffer + 2, 4, my_tag.uid.begin() + 4);
	my_tag.crc_uid[1] = buffer[6];
}

void CoreRFID::receiveUID2()
{
	std::array<uint8_t, 10> buffer;

	_interface.read(buffer.data(), buffer.size());

	setUID2(buffer.data());
}

void CoreRFID::setUID1(uint8_t *buffer)
{
	std::copy_n(buffer + 2, 4, my_tag.uid.begin());
	my_tag.crc_uid[0] = buffer[6];
}

void CoreRFID::receiveUID1()
{
	std::array<uint8_t, 10> buffer;

	_interface.read(buffer.data(), buffer.size());

	setUID1(buffer.data());
}

void CoreRFID::sendUID1()
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x08;
	const std::array<uint8_t, 2> cmd_tag = {0x93, 0x70};
	const std::array<uint8_t, 4> uid	 = {my_tag.uid[0], my_tag.uid[1], my_tag.uid[2], my_tag.uid[3]};
	const uint8_t uid_crc				 = my_tag.crc_uid[0];
	const uint8_t flags					 = 0x28;

	std::array<uint8_t, 10> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], uid[0],
									   uid[1],	   uid[2], uid[3],	   uid_crc,	   flags};

	send(command);
}

void CoreRFID::sendUID2()
{
	const uint8_t cmd_CR95HF			 = 0x04;
	const uint8_t length				 = 0x08;
	const std::array<uint8_t, 2> cmd_tag = {0x95, 0x70};
	const std::array<uint8_t, 4> uid	 = {my_tag.uid[4], my_tag.uid[5], my_tag.uid[6], my_tag.uid[7]};
	const uint8_t uid_crc				 = my_tag.crc_uid[1];
	const uint8_t flags					 = 0x28;

	std::array<uint8_t, 10> command = {cmd_CR95HF, length, cmd_tag[0], cmd_tag[1], uid[0],
									   uid[1],	   uid[2], uid[3],	   uid_crc,	   flags};

	send(command);
}

void CoreRFID::setSAK1(uint8_t *buffer)
{
	std::copy_n(buffer + 2, 2, my_tag.sak.begin());
}

void CoreRFID::receiveSAK1()
{
	std::array<uint8_t, 8> buffer;

	_interface.read(buffer.data(), buffer.size());

	setSAK1(buffer.data());
}

void CoreRFID::setSAK2(uint8_t *buffer)
{
	std::copy_n(buffer + 2, 2, my_tag.sak.begin() + 2);
}

void CoreRFID::receiveSAK2()
{
	std::array<uint8_t, 8> buffer;

	_interface.read(buffer.data(), buffer.size());

	setSAK2(buffer.data());
}

void CoreRFID::authentication()
{
	const uint8_t cmd_CR95HF				  = 0x04;
	const uint8_t length					  = 0x06;
	const uint8_t cmd_tag					  = 0x1B;
	const std::array<uint8_t, 4> tag_password = {0xFF, 0xFF, 0xFF, 0xFF};
	const uint8_t flags						  = 0x28;

	std::array<uint8_t, 8> command = {cmd_CR95HF,	   length,			cmd_tag,		 tag_password[0],
									  tag_password[1], tag_password[2], tag_password[3], flags};

	send(command);
}

auto CoreRFID::checkAuthentication(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> PASSWORD_CHECK_VALUES = {0xA0, 0x1E};

	if (std::array<uint8_t, 2> PASSWORD_tag_answer = {buffer[4], buffer[5]};
		(PASSWORD_tag_answer[0] == PASSWORD_CHECK_VALUES[0]) && (PASSWORD_tag_answer[1] == PASSWORD_CHECK_VALUES[1])) {
		return true;
	}

	return false;
}

auto CoreRFID::receiveAuthentication() -> bool
{
	std::array<uint8_t, 9> buffer;

	_interface.read(buffer.data(), buffer.size());

	return checkAuthentication(buffer.data());
}

void CoreRFID::readTagData()
{
	const uint8_t cmd_CR95HF	   = 0x04;
	const uint8_t length		   = 0x03;
	const uint8_t cmd_tag		   = 0x30;
	const uint8_t register_to_read = 0x05;
	const uint8_t flags			   = 0x28;

	std::array<uint8_t, 5> command = {cmd_CR95HF, length, cmd_tag, register_to_read, flags};

	send(command);
}

auto CoreRFID::checkCRC(const std::array<uint8_t, 21> &buffer) const -> bool
{
	std::array<uint8_t, 2> expected_crc = {buffer[18], buffer[19]};

	std::array<uint8_t, 16> data {0};
	std::copy_n(buffer.begin() + 2, 16, data.begin());

	if (std::array<uint8_t, 2> actual_crc = computeCRC(data.data(), data.size()); expected_crc == actual_crc) {
		return true;
	}
	return false;
}

auto CoreRFID::computeCRC(uint8_t const *data, size_t size) const -> std::array<uint8_t, 2>
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

void CoreRFID::setData(uint8_t *buffer)
{
	std::copy_n(buffer + 2, 16, my_tag.data.begin());
}

void CoreRFID::receiveTagData()
{
	std::array<uint8_t, 21> buffer;

	_interface.read(buffer.data(), buffer.size());

	if (checkCRC(buffer)) {
		setData(buffer.data());
	}
}

}	// namespace leka
