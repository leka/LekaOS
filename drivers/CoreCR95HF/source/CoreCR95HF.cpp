// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreCR95HF::setProtocolISO14443()
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

	receive(buffer);

	return checkAnswerSetup(buffer);
}

auto CoreCR95HF::checkAnswerSetup(const std::array<uint8_t, 2> &buffer) const -> bool
{
	const std::array<uint8_t, 2> CR95HF_setup_completed = {0x00, 0x00};

	return (buffer == CR95HF_setup_completed) ? true : false;
};

auto CoreCR95HF::init() -> bool
{
	setProtocolISO14443();

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

auto CoreCR95HF::calculateCommandSize(const size_t size) const -> size_t
{
	return size + 2;
}
}	// namespace leka
