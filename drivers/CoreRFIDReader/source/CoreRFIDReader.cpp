// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFIDReader.h"

#include "rtos/ThisThread.h"
using namespace std::chrono;

namespace leka {

void CoreRFIDReader::init()
{
	_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
	registerOnDataAvailableCallback();
}

void CoreRFIDReader::registerOnTagDetectedCallback(mbed::Callback<void()> callback)
{
	_on_tag_detected = callback;
};

void CoreRFIDReader::registerOnTagValidCallback(mbed::Callback<void()> callback)
{
	_on_tag_valid = callback;
};

void CoreRFIDReader::registerOnDataAvailableCallback()
{
	auto func	  = [this]() { this->onDataAvailable(); };
	auto callback = [this, func] { _event_queue.call(func); };
	_serial.sigio(callback);
}

void CoreRFIDReader::onDataAvailable()
{
	read();
	_on_tag_detected();
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

void CoreRFIDReader::setCommunicationProtocol(rfid::Protocol protocol)
{
	if (protocol == rfid::Protocol::ISO14443A) {
		setProtocolISO14443A();
		setGainAndModulationISO14443A();
	}
}

void CoreRFIDReader::setProtocolISO14443A()
{
	_serial.write(rfid::command::frame::set_protocol_iso14443.data(),
				  rfid::command::frame::set_protocol_iso14443.size());
}

void CoreRFIDReader::setGainAndModulationISO14443A()
{
	_serial.write(rfid::command::frame::set_gain_and_modulation.data(),
				  rfid::command::frame::set_gain_and_modulation.size());
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
		_tx_buf.at(i + rfid::tag_answer::heading_size) = cmd[i];
	}

	return cmd.size() + rfid::tag_answer::heading_size;
}

auto CoreRFIDReader::didTagCommunicationSucceed(size_t sizeTagData) -> bool
{
	uint8_t status = _rx_buf[0];
	uint8_t length = _rx_buf[1];

	auto didCommunicationSucceed = status == rfid::status::communication_succeed;
	auto didCommandSameSize		 = sizeTagData == length - rfid::tag_answer::flag_size;

	return (didCommunicationSucceed && didCommandSameSize);
}

void CoreRFIDReader::getDataFromTag(std::span<uint8_t> data)
{
	for (auto i = 0; i < data.size(); ++i) {
		data[i] = _rx_buf[i + rfid::tag_answer::heading_size];
	}
}

auto CoreRFIDReader::getTagData() -> rfid::Tag
{
	for (size_t i = 0; i < 18; ++i) {
		_tag.data[i] = _rx_buf[i + rfid::tag_answer::heading_size];
	}
	return _tag;
}

void CoreRFIDReader::onTagValid()
{
	_on_tag_valid();
}

}	// namespace leka
