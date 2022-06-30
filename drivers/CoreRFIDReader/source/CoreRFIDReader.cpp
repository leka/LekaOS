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
	setTagDetectionMode();
	log_debug("reader_init");
}

void CoreRFIDReader::registerTagAvailableCallback(mbed::Callback<void()> callback)
{
	_tagAvailableCallback = callback;
};

void CoreRFIDReader::registerOnATQARequestCallback(mbed::Callback<void()> callback)
{
	_on_atqa_received_callback = callback;
};

void CoreRFIDReader::registerOnRegisterCallback(mbed::Callback<void()> callback)
{
	_on_register_received_callback = callback;
};

void CoreRFIDReader::registerOnTagValidCallback(mbed::Callback<void()> callback)
{
	_on_tag_valid = callback;
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
	log_debug("read");
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

	didsetCommunicationProtocolSucceed();
}

void CoreRFIDReader::setGainAndModulationISO14443A()
{
	_serial.write(rfid::command::frame::set_gain_and_modulation.data(),
				  rfid::command::frame::set_gain_and_modulation.size());

	didsetCommunicationProtocolSucceed();
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
	log_debug("sendCommandToTag");
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

auto CoreRFIDReader::dataFromTagIsCorrect(size_t sizeTagData) -> bool
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

auto CoreRFIDReader::isTagSignatureValid() -> bool
{
	return (_tag.data[0] == 0x4C && _tag.data[1] == 0x45 && _tag.data[2] == 0x4B && _tag.data[3] == 0x41);
}

auto CoreRFIDReader::receiveATQA() -> bool
{
	std::array<uint8_t, 2> ATQA_answer {};
	std::span<uint8_t> span = {ATQA_answer};

	if (dataFromTagIsCorrect(span.size())) {
		copyTagDataToSpan(span);
	}

	_on_atqa_received_callback();
	log_debug("receiveAtqa");

	return (span[0] == ATQA_answer[0] && span[1] == ATQA_answer[1]);
}

auto CoreRFIDReader::receiveReadTagData() -> bool
{
	std::span<uint8_t> span = {_tag_data};
	if (dataFromTagIsCorrect(span.size())) {
		copyTagDataToSpan(span);
	}

	for (size_t i = 0; i < span.size(); ++i) {
		_tag.data[i] = span.data()[i];
	}
	log_debug("receive_register");

	std::array<uint8_t, 2> received_crc = {span[16], span[17]};

	_on_register_received_callback();

	return received_crc == computeCRC(span.data());
}

auto CoreRFIDReader::computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>
{
	uint32_t wCrc = 0x6363;
	size_t size	  = 16;

	do {
		std::byte bt;
		bt	 = static_cast<std::byte>(*data++);
		bt	 = (bt ^ static_cast<std::byte>(wCrc & 0x00FF));
		bt	 = (bt ^ (bt << 4));
		wCrc = (wCrc >> 8) ^ (static_cast<uint32_t>(bt) << 8) ^ (static_cast<uint32_t>(bt) << 3) ^
			   (static_cast<uint32_t>(bt) >> 4);
	} while (--size);

	std::array<uint8_t, 2> pbtCrc = {static_cast<uint8_t>(wCrc & 0xFF), static_cast<uint8_t>((wCrc >> 8) & 0xFF)};
	return pbtCrc;
}

void CoreRFIDReader::onTagValid()
{
	_on_tag_valid();
}

}	// namespace leka
