// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialCommunication.h"

void SerialCommunication::checkNewMessage()
{
	_reception_string.clear();

	while (_serial.readable()) {
		std::array<char, 1> temp_char {};

		_serial.read(temp_char.data(), 1);

		_reception_string.push_back(temp_char[0]);
	}

	auto gotNewMessage = !_reception_string.empty();
	if (gotNewMessage) {
		_event_flags.set(NEW_MESSAGE);
	}
}

auto SerialCommunication::getMessage() -> std::filesystem::path
{
	return std::filesystem::path {_reception_string};
}

void SerialCommunication::sendMessage(const std::string_view &message)
{
	_serial.write(message.data(), message.length());
}
