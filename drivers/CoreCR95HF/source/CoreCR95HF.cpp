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

	return buffer == cr95hf::status::setup_success ? true : false;
}

auto CoreCR95HF::receiveCR95HFAnswer() -> size_t
{
	size_t size {0};

	if (isDataAvailable()) {
		size = _serial.read(_rx_buf.data(), _rx_buf.size());
	}

	return size;
}

auto CoreCR95HF::isDataAvailable() -> bool
{
	int count = 0;
	while (!_serial.readable()) {
		++count;
		rtos::ThisThread::sleep_for(1ms);

		if (count > 10) {
			_rx_buf[0] = cr95hf::status::error_time_out;
			return false;
		}
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

void CoreCR95HF::send(const lstd::span<uint8_t> &command) {}

auto CoreCR95HF::receive(const lstd::span<uint8_t> &tag_anwser) -> size_t {}

}	// namespace leka
