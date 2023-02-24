// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreRFIDReaderCR95HF.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreRFIDReaderCR95HF::init()
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

void CoreRFIDReaderCR95HF::registerOnTagDetectedCallback(const std::function<void()> &callback)
{
	_on_tag_response_available = callback;
};

void CoreRFIDReaderCR95HF::registerOnTagReadableCallback(const std::function<void(rfid::Tag)> &callback)
{
	_on_tag_readable = callback;
};

void CoreRFIDReaderCR95HF::_receiveResponseFromTag()
{
	rtos::ThisThread::sleep_for(10ms);
	if (_serial.readable()) {
		_serial.read(_rx_buf.data(), _rx_buf.size());
	}
}

void CoreRFIDReaderCR95HF::setModeTagDetection()
{
	_serial.write(rfid::cr95hf::command::frame::set_tag_detection_mode.data(),
				  rfid::cr95hf::command::frame::set_tag_detection_mode.size());
	rtos::ThisThread::sleep_for(10ms);
}

void CoreRFIDReaderCR95HF::setCommunicationProtocol(rfid::Protocol protocol)
{
	if (protocol == rfid::Protocol::ISO14443A) {
		_setProtocolISO14443A();
		_setGainAndModulationISO14443A();
	}
}

void CoreRFIDReaderCR95HF::_setProtocolISO14443A()
{
	_serial.write(rfid::cr95hf::command::frame::set_protocol_iso14443.data(),
				  rfid::cr95hf::command::frame::set_protocol_iso14443.size());
}

void CoreRFIDReaderCR95HF::_setGainAndModulationISO14443A()
{
	_serial.write(rfid::cr95hf::command::frame::set_gain_and_modulation.data(),
				  rfid::cr95hf::command::frame::set_gain_and_modulation.size());
}

void CoreRFIDReaderCR95HF::sendRequestToTag(std::span<const uint8_t> data)
{
	_tx_buf.at(0) = rfid::cr95hf::command::send_receive;
	_tx_buf.at(1) = static_cast<uint8_t>(data.size());

	std::copy(data.begin(), data.end(), _tx_buf.begin() + rfid::cr95hf::tag_answer::heading_size);

	_serial.write(_tx_buf.data(), data.size() + rfid::cr95hf::tag_answer::heading_size);
}

auto CoreRFIDReaderCR95HF::didTagCommunicationSucceed(size_t sizeTagData) -> bool
{
	uint8_t status = _rx_buf.at(0);
	uint8_t length = _rx_buf.at(1);

	auto did_communication_succeed = status == rfid::cr95hf::status::communication_succeed;
	auto did_command_same_size	   = sizeTagData == length - rfid::cr95hf::tag_answer::flag_size;

	return (did_communication_succeed && did_command_same_size);
}

auto CoreRFIDReaderCR95HF::getTag() -> rfid::Tag &
{
	std::copy(_rx_buf.begin() + rfid::cr95hf::tag_answer::heading_size, _rx_buf.end(), _tag.data.begin());

	return _tag;
}

void CoreRFIDReaderCR95HF::onTagReadable()
{
	_on_tag_readable(_tag);
}

}	// namespace leka
