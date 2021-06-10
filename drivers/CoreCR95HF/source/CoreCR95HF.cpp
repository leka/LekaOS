// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"
#include <cstddef>
#include <cstdint>

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

auto CoreCR95HF::init() -> bool
{
	setProtocolISO14443();

	rtos::ThisThread::sleep_for(10ms);

	if (!didSetupSucceed()) {
		return false;
	}

	setGainAndModulation();

	rtos::ThisThread::sleep_for(10ms);

	if (!didSetupSucceed()) {
		return false;
	}

	return true;
}

auto CoreCR95HF::didSetupSucceed() -> bool
{
	receiveCR95HFAnswer();

	std::array<uint8_t, 2> buffer {_rx_buf[0], _rx_buf[1]};

	return buffer == rfid::cr95hf::status::setup_success ? true : false;
}

auto CoreCR95HF::receiveCR95HFAnswer() -> size_t
{
	size_t size {0};

	rtos::ThisThread::sleep_for(10ms);
	if (_serial.readable()) {
		size = _serial.read(_rx_buf.data(), _rx_buf.size());
	}

	return size;
}

void CoreCR95HF::setProtocolISO14443()
{
	_serial.write(rfid::cr95hf::command::frame::set_protocol_iso14443.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	_serial.write(rfid::cr95hf::command::frame::set_gain_and_modulation.data(), 6);
}

void CoreCR95HF::send(const lstd::span<uint8_t> &command)
{
	auto command_size = formatCommand(command);

	_serial.write(_tx_buf.data(), command_size);
}

auto CoreCR95HF::formatCommand(const lstd::span<uint8_t> &command) -> size_t
{
	_tx_buf[0] = rfid::cr95hf::command::send_receive;
	_tx_buf[1] = static_cast<uint8_t>(command.size());

	for (auto i = 0; i < command.size(); ++i) {
		_tx_buf[i + rfid::cr95hf::tag_answer::heading_size] = command[i];
	}

	return command.size() + rfid::cr95hf::tag_answer::heading_size;
}

auto CoreCR95HF::receive(const lstd::span<uint8_t> &answer) -> size_t
{
	auto size = receiveCR95HFAnswer();

	if (!processTagAnswer(answer, size)) {
		return 0;
	}

	return (size - rfid::cr95hf::tag_answer::heading_size - rfid::cr95hf::tag_answer::flag_size);
}

auto CoreCR95HF::processTagAnswer(const lstd::span<uint8_t> &answer, const size_t size) -> bool
{
	uint8_t status = _rx_buf[0];
	uint8_t length = _rx_buf[1];

	if (status != rfid::cr95hf::status::communication_succeed ||
		length != size - rfid::cr95hf::tag_answer::heading_size) {
		return false;
	}

	for (auto i = 0; i < length - rfid::cr95hf::tag_answer::flag_size; ++i) {
		answer.data()[i] = _rx_buf[i + rfid::cr95hf::tag_answer::heading_size];
	}

	return true;
}

}	// namespace leka
