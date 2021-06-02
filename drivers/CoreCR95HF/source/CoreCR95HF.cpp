// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

void CoreCR95HF::setProtocoleISO14443()
{
	std::array<uint8_t, 4> set_protocol_ISO14443_command {cr95hf::command::set_protocol, 0x02,
														  cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag};
	_serial.write(set_protocol_ISO14443_command.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	std::array<uint8_t, 6> set_gain_and_modulation_command {
		cr95hf::command::set_gain_and_modulation,
		0x04,
		cr95hf::arc_b,
		cr95hf::flag_increment,
		cr95hf::gain_modulation_index,
		static_cast<uint8_t>(cr95hf::protocol::ISO14443A.gain_modulation_values())};
	_serial.write(set_gain_and_modulation_command.data(), 6);
}

auto CoreCR95HF::receiveSetupAnswer() -> bool
{
	std::array<uint8_t, 2> buffer;

	_serial.read(buffer.data(), buffer.size());

	return checkSensorSetup(buffer);
}

auto CoreCR95HF::init() -> bool
{
	using namespace std::chrono;

	bool init_status = false;

	setProtocoleISO14443();
	rtos::ThisThread::sleep_for(10ms);
	if (init_status = receiveSetupAnswer(); init_status == false) {
		return init_status;
	}

	setGainAndModulation();
	rtos::ThisThread::sleep_for(10ms);
	init_status = receiveSetupAnswer();

	return init_status;
}

auto CoreCR95HF::calculateCommandSize(const size_t iso_cmd_size) const -> size_t
{
	return iso_cmd_size + 2;
}
}	// namespace leka
