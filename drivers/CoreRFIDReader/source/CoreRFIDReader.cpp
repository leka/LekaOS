// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFIDReader.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include "rtos/ThisThread.h"
using namespace std::chrono;

namespace leka {

void CoreRFIDReader::init()
{
	_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
	registerCallback();
}

void CoreRFIDReader::registerTagAvailableCallback(tag_available_callback_t callback)
{
	_tagAvailableCallback = callback;
};

void CoreRFIDReader::registerCallback()
{
	auto func	  = [this]() { this->onDataAvailable(); };
	auto callback = [this, func] { _event_queue.call(func); };
	_serial.sigio(callback);
}

void CoreRFIDReader::onDataAvailable()
{
	read();
	_tagAvailableCallback();
}

void CoreRFIDReader::read()
{
	rtos::ThisThread::sleep_for(10ms);
	if (_serial.readable()) {
		_anwser_size = _serial.read(_rx_buf.data(), _rx_buf.size());
	}
}

auto CoreRFIDReader::isTagDetected() -> bool
{
	std::array<uint8_t, 2> buffer {};

	if (_anwser_size != rfid::expected_answer_size::tag_detection) {
		return false;
	}

	std::copy(_rx_buf.begin() + 1, _rx_buf.begin() + 1 + buffer.size(), buffer.begin());

	return buffer == rfid::status::tag_detection_callback;
}

void CoreRFIDReader::setTagDetectionMode()
{
	_serial.write(rfid::command::frame::set_tag_detection_mode.data(),
				  rfid::command::frame::set_tag_detection_mode.size());
}

auto CoreRFIDReader::setBaudrate(uint8_t baudrate) -> bool
{
	std::array<uint8_t, 3> set_baudrate_frame = {rfid::command::set_baudrate::id, rfid::command::set_baudrate::length,
												 baudrate};

	_serial.write(set_baudrate_frame.data(), set_baudrate_frame.size());

	return (setBaudrateDidSucceed());
}

auto CoreRFIDReader::setBaudrateDidSucceed() -> bool
{
	read();
	if (_anwser_size != rfid::expected_answer_size::set_baudrate) {
		return false;
	}

	return _rx_buf[0] == 0x55;
}

auto CoreRFIDReader::setCommunicationProtocol(rfid::Protocol protocol) -> bool
{
	auto setCommunicationProtocol = bool {false};
	if (protocol == rfid::Protocol::ISO14443A) {
		setCommunicationProtocol = setProtocolISO14443A() && setGainAndModulationISO14443A();
	}

	return setCommunicationProtocol;
}

auto CoreRFIDReader::setProtocolISO14443A() -> bool
{
	_serial.write(rfid::command::frame::set_protocol_iso14443.data(),
				  rfid::command::frame::set_protocol_iso14443.size());

	return didsetCommunicationProtocolSucceed();
}

auto CoreRFIDReader::setGainAndModulationISO14443A() -> bool
{
	_serial.write(rfid::command::frame::set_gain_and_modulation.data(),
				  rfid::command::frame::set_gain_and_modulation.size());

	return didsetCommunicationProtocolSucceed();
}

auto CoreRFIDReader::didsetCommunicationProtocolSucceed() -> bool
{
	if (_anwser_size != rfid::expected_answer_size::set_communication_protocol) {
		return false;
	}

	std::array<uint8_t, 2> buffer {_rx_buf[0], _rx_buf[1]};

	return buffer == rfid::status::setup_success;
}

void CoreRFIDReader::sendCommandToTag(std::span<const uint8_t> cmd)
{
	auto command_size = formatCommand(cmd);

	_serial.write(_tx_buf.data(), command_size);
}

auto CoreRFIDReader::formatCommand(std::span<const uint8_t> cmd) -> size_t
{
	_tx_buf[0] = rfid::command::send_receive;
	_tx_buf[1] = static_cast<uint8_t>(cmd.size());

	for (auto i = 0; i < cmd.size(); ++i) {
		_tx_buf[i + rfid::tag_answer::heading_size] = cmd[i];
	}

	return cmd.size() + rfid::tag_answer::heading_size;
}

auto CoreRFIDReader::receiveDataFromTag(std::span<uint8_t> data) -> bool
{
	if (!DataFromTagIsCorrect(data.size())) {
		return false;
	}

	copyTagDataToSpan(data);

	return true;
}

auto CoreRFIDReader::DataFromTagIsCorrect(size_t sizeTagData) -> bool
{
	uint8_t status = _rx_buf[0];
	uint8_t length = _rx_buf[1];

	return (status == rfid::status::communication_succeed && sizeTagData == length - rfid::tag_answer::flag_size);
}

void CoreRFIDReader::copyTagDataToSpan(std::span<uint8_t> data)
{
	for (auto i = 0; i < data.size(); ++i) {
		data[i] = _rx_buf[i + rfid::tag_answer::heading_size];
	}
}

}	// namespace leka
