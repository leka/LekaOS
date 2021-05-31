// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

void CoreCR95HF::receive(uint8_t *data, size_t size)
{
	_serial.read(_rx_buf.data(), size);

	for (auto i = 0; i < size; ++i) {
		data[i] = _rx_buf[i];
	}
}

void CoreCR95HF::setProcoleISO14443()
{
	std::array<uint8_t, 4> set_protocol_ISO14443_command {cr95hf::command::set_protocol, 0x02,
														  cr95hf::protocol::ISO14443A, cr95hf::protocol_flag};
	_serial.write(set_protocol_ISO14443_command.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	std::array<uint8_t, 6> set_gain_and_modulation_command {cr95hf::command::set_gain_and_modulation,
															0x04,
															cr95hf::ARC_B_register,
															cr95hf::flag_increment,
															cr95hf::gain_modulation_index,
															cr95hf::gain_modulation_values::ISO14443A};
	_serial.write(set_gain_and_modulation_command.data(), 6);
}

auto CoreCR95HF::receiveSetupAnswer() -> bool
{
	std::array<uint8_t, 2> buffer;

	_serial.read(buffer.data(), buffer.size());

	return checkSensorSetup(buffer.data());
}

auto CoreCR95HF::checkSensorSetup(const uint8_t *buffer) const -> bool
{
	const std::array<uint8_t, 2> CR95HF_setup_completed = {0x00, 0x00};

	if (std::array<uint8_t, 2> CR95HF_answer = {buffer[0], buffer[1]};
		CR95HF_answer[0] == CR95HF_setup_completed[0] && CR95HF_answer[1] == CR95HF_setup_completed[1]) {
		return true;
	}

	return false;
}

auto CoreCR95HF::init() -> bool
{
	using namespace std::chrono;

	bool init_status = false;

	setProcoleISO14443();
	rtos::ThisThread::sleep_for(10ms);
	if (init_status = receiveSetupAnswer(); init_status == false) {
		return init_status;
	}

	setGainAndModulation();
	rtos::ThisThread::sleep_for(10ms);
	init_status = receiveSetupAnswer();

	return init_status;
}

}	// namespace leka
