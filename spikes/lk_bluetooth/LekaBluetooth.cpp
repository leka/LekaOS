// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaBluetooth.h"

using namespace mbed;
using namespace std::chrono;

Bluetooth::Bluetooth()
	: _interface(BT_UART_TX, BT_UART_RX, 115200), _bluetooth_reset(BT_RESET), _bluetooth_wake_up(BT_WAKE_UP)
{
	// rtos::ThisThread::sleep_for(1s);
	_bluetooth_wake_up = 1;
	rtos::ThisThread::sleep_for(25ms);	 // Delay recommanded in datasheet
	_bluetooth_reset = 1;
	rtos::ThisThread::sleep_for(450ms);	  // Delay recommanded in datasheet

	_buffer_length =
		BM64::getCommand(BM64::ComplexCommand::eq_soft_mode, BM64::ComplexCommand::eq_soft_mode_length, _buffer);
	_interface.write(_buffer.data(), _buffer_length);
	rtos::ThisThread::sleep_for(100ms);
}

void Bluetooth::pairing()
{
	auto max_attempts = 10;
	auto attempts	  = 0;
	_buffer_length	  = 0;

	_buffer_length = BM64::getCommand(BM64::ComplexCommand::pairing, BM64::ComplexCommand::pairing_length, _buffer);

	while (attempts < max_attempts) {
		_interface.write(_buffer.data(), _buffer_length);
		checkResponse();
		if (_paired) {
			break;
		}

		rtos::ThisThread::sleep_for(10s);

		attempts++;
	}
}

auto Bluetooth::checkResponse(bool printResponse) -> bool
{
	auto attempts	  = 0;
	auto max_attempts = 100;
	_buffer_length	  = 0;

	auto still_alive = false;

	while (attempts < max_attempts) {
		rtos::ThisThread::sleep_for(10ms);
		while (_interface.readable()) {
			_interface.read(_buffer.data(), 1);
			if (_buffer.at(0) == BM64::Command::SINC_WORD) {
				_interface.read(_buffer.data() + 1, 2);
				_buffer_length = (static_cast<uint16_t>(_buffer.at(1) << 8)) + static_cast<uint16_t>(_buffer.at(2)) +
								 1;	  // +1 for checksum

				rtos::ThisThread::sleep_for(10ms);	 // Delay to catch length
				if (_buffer_length + 1 + 2 > MAX_BUFFER_SIZE) {
					return still_alive;
				}
				still_alive = true;

				_interface.read(_buffer.data() + 1 + 2, _buffer_length);
				_buffer_length += 1 + 2;   // SINC_WORD + 2 bytes for length

				if (_buffer.at(3) == BM64::Command::BTM_STATUS &&
					_buffer.at(4) == BM64::Command::A2DP_LINK_ESTABLISHED) {
					_paired = true;
				} else if (_buffer.at(3) == BM64::Command::DATA_INCOMING) {	  // Value for message
					const auto n_characters_wrapper = 11;
					_msg_length = _buffer_length > MAX_BUFFER_SIZE ? MAX_BUFFER_SIZE - n_characters_wrapper
																   : _buffer_length - n_characters_wrapper;
					for (auto i = 0; i < _msg_length; i++) {
						_received_message.at(i) = _buffer.at(i + 10);
					}
					_new_message = true;
				}

				if (printResponse) {
					printf("Frame received = ");
					for (int i = 0; i < _buffer_length; i++) {
						printf("%X ", _buffer.at(i));
					}
					printf("\n");
				}
			}
			// attempts = 0;
		}
		attempts++;
	}

	return still_alive;
}

void Bluetooth::playPause()
{
	/* Pause or play video/song on paired device */

	_buffer_length = 0;

	_buffer_length =
		BM64::getCommand(BM64::ComplexCommand::play_pause, BM64::ComplexCommand::play_pause_length, _buffer);

	_interface.write(_buffer.data(), _buffer_length);
}

auto Bluetooth::isPaired() const -> bool
{
	return _paired;
}

void Bluetooth::sendMessage(const char *msg, size_t msg_length)
{
	_buffer_length				  = 0;
	const auto command_header	  = 1 + 1 + 1 + 2 + 2;
	const uint16_t msg_cmd_length = command_header + msg_length;
	std::array<uint8_t, 128> msg_cmd {
		BM64::Command::SEND_DATA,
		0x00,
		0x00,
		static_cast<uint8_t>((static_cast<uint16_t>(msg_length >> 8))),
		static_cast<uint8_t>(msg_length),
		static_cast<uint8_t>((static_cast<uint16_t>(msg_length >> 8))),
		static_cast<uint8_t>(msg_length)};	 // Header of the command, check BM64 docs for more details

	// Copy message
	for (size_t i = 0; i < msg_length; i++) {
		msg_cmd.at(i + command_header) = msg[i];
	}

	_buffer_length = BM64::getCommand(msg_cmd, msg_cmd_length, _buffer);

	_interface.write(_buffer.data(), _buffer_length);
}

auto Bluetooth::getMessage(std::span<char> buffer) -> int
{
	for (auto i = 0; i < _msg_length; i++) {
		buffer[i]				= _received_message.at(i);
		_received_message.at(i) = 0;
	}
	_new_message = false;
	return _msg_length;
}

auto Bluetooth::checkNewMessage() const -> bool
{
	return _new_message;
}

void Bluetooth::start()
{
	std::array<char, MAX_BUFFER_SIZE> text {};
	auto text_length = 0;

	auto counter				 = 0;
	constexpr auto counter_limit = 5;

	log_info("Bluetooth example\n");

	// Enable sound output
	_buffer_length =
		BM64::getCommand(BM64::ComplexCommand::eq_soft_mode, BM64::ComplexCommand::eq_soft_mode_length, _buffer);
	_interface.write(_buffer.data(), _buffer_length);
	rtos::ThisThread::sleep_for(100ms);

	while (!isPaired()) {
		log_info("Bluetooth is trying to pair...");
		pairing();
		rtos::ThisThread::sleep_for(1s);
	}
	text_length = sprintf(text.data(), "This spike do an echo via bluetooth communication.\n");
	sendMessage(text.data(), text_length);
	text_length =
		sprintf(text.data(), "You also can connect with your phone and play songs via the robot!\nEnjoy!\n\n");
	sendMessage(text.data(), text_length);

	while (true) {
		log_info("Counter: %d/%d", counter, counter_limit);
		if (checkResponse(true)) {
			counter = 0;
		}
		if (checkNewMessage()) {
			text_length = getMessage(text);
			rtos::ThisThread::sleep_for(2s);		 // Delay of echo
			sendMessage(text.data(), text_length);	 // ECHO

		} else if (counter > counter_limit) {
			log_info("Bluetooth is trying to pair...");
			pairing();
		}
		counter++;
		rtos::ThisThread::sleep_for(1s);
	}

	log_info("End of Bluetooth example\n");
}
