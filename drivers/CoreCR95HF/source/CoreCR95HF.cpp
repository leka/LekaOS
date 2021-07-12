// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <lstd_span>

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreCR95HF::registerCallback()
{
	static auto *self = this;
	auto callback	  = []() { self->onTagAvailable(); };
	_serial.sigio(callback);
}

void CoreCR95HF::onTagAvailable()
{
	read();

	if (receiveTagDetectionCallback()) {
		_tagAvailableCallback();
	}

	setModeTagDetection();
}

void CoreCR95HF::read()
{
	rtos::ThisThread::sleep_for(10ms);
	if (_serial.readable()) {
		_anwser_size = _serial.read(_rx_buf.data(), _rx_buf.size());
	}
}

auto CoreCR95HF::receiveTagDetectionCallback() -> bool
{
	std::array<uint8_t, 2> buffer {};

	if (_anwser_size != 3) {
		return false;
	}

	std::copy(_rx_buf.begin() + 1, _rx_buf.begin() + 1 + buffer.size(), buffer.begin());

	return buffer == rfid::cr95hf::status::tag_detection_callback ? true : false;
}

void CoreCR95HF::setModeTagDetection()
{
	_serial.write(rfid::cr95hf::command::frame::set_mode_tag_detection.data(),
				  rfid::cr95hf::command::frame::set_mode_tag_detection.size());
}

std::array<uint8_t, 17> CoreCR95HF::getIDN()
{
	std::array<uint8_t, 17> idn {};

	askCR95HFForIDN();
	if (!didIDNIsCorrect()) {
		return idn;
	}

	std::copy(_rx_buf.begin(), _rx_buf.begin() + idn.size(), idn.begin());

	return idn;
}

void CoreCR95HF::askCR95HFForIDN()
{
	_serial.write(rfid::cr95hf::command::frame::idn.data(), rfid::cr95hf::command::frame::idn.size());
}

auto CoreCR95HF::didIDNIsCorrect() -> bool
{
	if (_anwser_size != 17) {
		return false;
	}

	std::array<uint8_t, 2> buffer {_rx_buf[0], _rx_buf[1]};

	return buffer == rfid::cr95hf::status::idn_success ? true : false;
}

auto CoreCR95HF::setBaudrate(uint8_t baudrate) -> bool
{
	std::array<uint8_t, 3> set_baudrate_frame = {rfid::cr95hf::command::set_baudrate::id,
												 rfid::cr95hf::command::set_baudrate::length, baudrate};

	_serial.write(set_baudrate_frame.data(), set_baudrate_frame.size());

	if (!didSetBaudrateSucceed(baudrate)) {
		return false;
	}
	return true;
}

auto CoreCR95HF::didSetBaudrateSucceed(uint8_t baudrate) -> bool
{
	if (_anwser_size != 1) {
		return false;
	}

	return _rx_buf[0] == baudrate ? true : false;
}

auto CoreCR95HF::setCommunicationProtocol(rfid::Protocol protocol) -> bool
{
	auto setCommunicationProtocol = bool {false};
	if (protocol == rfid::Protocol::ISO14443A) {
		setCommunicationProtocol = setProtocolISO14443A() && setGainAndModulationISO14443A();
	}

	return setCommunicationProtocol;
}

auto CoreCR95HF::setProtocolISO14443A() -> bool
{
	_serial.write(rfid::cr95hf::command::frame::set_protocol_iso14443.data(),
				  rfid::cr95hf::command::frame::set_protocol_iso14443.size());

	return didsetCommunicationProtocolSucceed();
}

auto CoreCR95HF::setGainAndModulationISO14443A() -> bool
{
	_serial.write(rfid::cr95hf::command::frame::set_gain_and_modulation.data(),
				  rfid::cr95hf::command::frame::set_gain_and_modulation.size());

	return didsetCommunicationProtocolSucceed();
}

auto CoreCR95HF::didsetCommunicationProtocolSucceed() -> bool
{
	if (_anwser_size != 2) {
		return false;
	}

	std::array<uint8_t, 2> buffer {_rx_buf[0], _rx_buf[1]};

	return buffer == rfid::cr95hf::status::setup_success ? true : false;
}

void CoreCR95HF::sendCommandToTag(lstd::span<uint8_t> command)
{
	auto command_size = formatCommand(command);

	_serial.write(_tx_buf.data(), command_size);
}

auto CoreCR95HF::formatCommand(lstd::span<uint8_t> command) -> size_t
{
	_tx_buf[0] = rfid::cr95hf::command::send_receive;
	_tx_buf[1] = static_cast<uint8_t>(command.size());

	for (auto i = 0; i < command.size(); ++i) {
		_tx_buf[i + rfid::cr95hf::tag_answer::heading_size] = command[i];
	}

	return command.size() + rfid::cr95hf::tag_answer::heading_size;
}

auto CoreCR95HF::receiveDataFromTag(lstd::span<uint8_t> data) -> size_t
{
	if (!DataFromTagIsCorrect(data.size())) {
		return 0;
	}

	copyTagDataToSpan(data);

	return true;
}

auto CoreCR95HF::DataFromTagIsCorrect(size_t sizeTagData) -> bool
{
	uint8_t status = _rx_buf[0];
	uint8_t length = _rx_buf[1];

	if (status != rfid::cr95hf::status::communication_succeed ||
		length != sizeTagData - rfid::cr95hf::tag_answer::flag_size) {
		return false;
	}

	return true;
}

void CoreCR95HF::copyTagDataToSpan(lstd::span<uint8_t> data)
{
	for (auto i = 0; i < data.size(); ++i) {
		data[i] = _rx_buf[i + rfid::cr95hf::tag_answer::heading_size];
	}
}

}	// namespace leka
