// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialCommunication.h"

void SerialCommunication::checkNewMessage()
{
	_reception_string.clear();

	while (_serial.readable()) {
		auto c = char {};

		_serial.read(&c, 1);

		_reception_string.push_back(c);
	}

	auto gotNewMessage = !_reception_string.empty();
	if (gotNewMessage) {
		_event_flags.set(NEW_MESSAGE);
	}
}

auto SerialCommunication::getMessage() const -> std::filesystem::path
{
	return std::filesystem::path {_reception_string};
}

void SerialCommunication::sendMessage(const std::string_view &message)
{
	_serial.write(message.data(), message.length());
}
