// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <span>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BM64Converter.h"
#include "LogKit.h"

inline constexpr uint8_t MAX_BUFFER_SIZE = 128;

class Bluetooth
{
  public:
	Bluetooth();
	~Bluetooth() = default;

	void start();
	void pairing();
	void playPause();
	auto checkResponse(bool printResponse = false) -> bool;

	[[nodiscard]] auto isPaired() const -> bool;
	void sendMessage(const char *msg, std::size_t msg_length);
	auto getMessage(std::span<char> buffer) -> int;
	[[nodiscard]] auto checkNewMessage() const -> bool;

  private:
	mbed::BufferedSerial _interface;
	mbed::DigitalOut _bluetooth_reset;
	mbed::DigitalOut _bluetooth_wake_up;

	std::array<uint8_t, MAX_BUFFER_SIZE> _buffer;
	uint16_t _buffer_length = 0;
	std::array<char, MAX_BUFFER_SIZE> _received_message;
	int _msg_length = 0;

	bool _paired	  = false;	 // Check simultaneous pairing is possible
	bool _new_message = true;
};
