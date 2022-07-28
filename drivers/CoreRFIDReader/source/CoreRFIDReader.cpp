// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFIDReader.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreRFIDReader::init()
{
	_event_queue.dispatch_forever();

	// ? BufferedSerial::sigio executes the callback in ISR context. We use an
	// ? event queue to defer the execution of the callback outside the ISR context.

	auto callback = [this] {
		_event_queue.call([this]() {
			this->_receiveResponseFromTag();
			_on_tag_response_available();
		});
	};
	_serial.sigio(callback);
}

void CoreRFIDReader::registerOnTagDetectedCallback(const std::function<void()> &callback)
{
	_on_tag_response_available = callback;
};

void CoreRFIDReader::registerOnTagReadableCallback(const std::function<void(rfid::Tag &)> &callback)
{
	_on_tag_readable = callback;
};

void CoreRFIDReader::_receiveResponseFromTag()
{
	rtos::ThisThread::sleep_for(10ms);
	if (_serial.readable()) {
		_serial.read(_rx_buf.data(), _rx_buf.size());
	}
}

void CoreRFIDReader::setModeTagDetection()
{
	_serial.write(rfid::command::frame::set_tag_detection_mode.data(),
				  rfid::command::frame::set_tag_detection_mode.size());
	rtos::ThisThread::sleep_for(10ms);
}

void CoreRFIDReader::setCommunicationProtocol(rfid::Protocol protocol)
{
	if (protocol == rfid::Protocol::ISO14443A) {
		_setProtocolISO14443A();
		_setGainAndModulationISO14443A();
	}
}

void CoreRFIDReader::_setProtocolISO14443A()
{
	_serial.write(rfid::command::frame::set_protocol_iso14443.data(),
				  rfid::command::frame::set_protocol_iso14443.size());
}

void CoreRFIDReader::_setGainAndModulationISO14443A()
{
	_serial.write(rfid::command::frame::set_gain_and_modulation.data(),
				  rfid::command::frame::set_gain_and_modulation.size());
}

void CoreRFIDReader::sendRequestToTag(std::span<const uint8_t> data)
{
	_tx_buf.at(0) = rfid::command::send_receive;
	_tx_buf.at(1) = static_cast<uint8_t>(data.size());

	for (auto i = 0; i < data.size(); ++i) {
		_tx_buf.at(i + rfid::tag_answer::heading_size) = data[i];
	}

	_serial.write(_tx_buf.data(), data.size() + rfid::tag_answer::heading_size);
}

auto CoreRFIDReader::didTagCommunicationSucceed(size_t sizeTagData) -> bool
{
	uint8_t status = _rx_buf.at(0);
	uint8_t length = _rx_buf.at(1);

	auto did_communication_succeed = status == rfid::status::communication_succeed;
	auto did_command_same_size	   = sizeTagData == length - rfid::tag_answer::flag_size;

	return (did_communication_succeed && did_command_same_size);
}

auto CoreRFIDReader::getTag() -> rfid::Tag &
{
	for (auto i = 0; i < _tag.data.size(); ++i) {
		_tag.data.at(i) = _rx_buf.at(i + rfid::tag_answer::heading_size);
	}
	return _tag;
}

void CoreRFIDReader::onTagReadable()
{
	_on_tag_readable(_tag);
}

}	// namespace leka
