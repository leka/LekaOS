// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

void CoreCR95HF::setProtocoleISO14443()
{
	_serial.write(cr95hf::command::set_protocol_ISO14443_command.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	_serial.write(cr95hf::command::set_gain_and_modulation_command.data(), 6);
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
