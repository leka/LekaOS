// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

using namespace std::chrono;

namespace leka {

void CoreCR95HF::setProtocoleISO14443()
{
	_serial.write(cr95hf::command::set_protocol_ISO14443_command.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	_serial.write(cr95hf::command::set_gain_and_modulation_command.data(), 6);
}

auto CoreCR95HF::isSetupAnswerCorrect() -> bool
{
	std::array<uint8_t, 2> buffer;

	_serial.read(buffer.data(), buffer.size());

	return checkSensorSetup(buffer);
}

auto CoreCR95HF::init() -> bool
{
	setProtocoleISO14443();

	rtos::ThisThread::sleep_for(10ms);

	if (!isSetupAnswerCorrect()) {
		return false;
	}

	setGainAndModulation();

	rtos::ThisThread::sleep_for(10ms);

	if (!isSetupAnswerCorrect()) {
		return false;
	}

	return true;
}

auto CoreCR95HF::calculateCommandSize(const size_t iso_cmd_size) const -> size_t
{
	return iso_cmd_size + 2;
}
}	// namespace leka
