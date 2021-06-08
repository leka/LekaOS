// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"
#include <cstddef>
#include <cstdint>

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreCR95HF::send(const lstd::span<uint8_t> &command)
{
	auto command_size = formatCommand(command);

	_serial.write(_tx_buf.data(), command_size);
}

auto CoreCR95HF::formatCommand(const lstd::span<uint8_t> &command) -> size_t
{
	_tx_buf[0] = cr95hf::command::send_receive;
	_tx_buf[1] = command.size();

	for (auto i = 0; i < command.size(); ++i) {
		_tx_buf[i + cr95hf::tag_answer_heading_size] = command[i];
	}

	return command.size() + cr95hf::tag_answer_heading_size;
}

auto CoreCR95HF::didSetupSucceed() -> bool
{
	receiveCR95HFAnswer();

	std::array<uint8_t, 2> buffer {_rx_buf[0], _rx_buf[1]};

	return buffer == cr95hf::status::setup_success ? true : false;
}

auto CoreCR95HF::receiveCR95HFAnswer() -> size_t
{
	auto size = _serial.read(_rx_buf.data(), _rx_buf.size());

	if (size < 0) {	  // serial::read error
		return 0;
	}

	return size;
}

auto CoreCR95HF::receive(const lstd::span<uint8_t> &tag_anwser) -> size_t
{
	auto size = _serial.read(_rx_buf.data(), _rx_buf.size());

	if (size < 0) {	  // serial::read error
		return 0;
	}

	if (!formatTagAnswer(tag_anwser, size)) {
		return 0;
	}

	return size;
}

auto CoreCR95HF::formatTagAnswer(const lstd::span<uint8_t> &tag_anwser, const size_t size) -> bool
{
	uint8_t status = _rx_buf[0];
	uint8_t length = _rx_buf[1];

	if (status != cr95hf::status::communication_succeed || length != size - cr95hf::tag_answer_heading_size) {
		return false;
	};

	for (auto i = 0; i < length - cr95hf::tag_answer_flag_size; ++i) {
		tag_anwser.data()[i] = _rx_buf[i + cr95hf::tag_answer_heading_size];
	}

	return true;
}

void CoreCR95HF::setProtocolISO14443()
{
	_serial.write(cr95hf::command::frame::set_protocol_iso14443.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	_serial.write(cr95hf::command::frame::set_gain_and_modulation.data(), 6);
}

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

}	// namespace leka
