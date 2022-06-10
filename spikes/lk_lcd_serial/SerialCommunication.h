// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <string>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/EventFlags.h"

#define NEW_MESSAGE (1UL << 10)

class SerialCommunication
{
  public:
	explicit SerialCommunication(rtos::EventFlags &event_flags) : _event_flags(event_flags) {}

	void checkNewMessage();

	[[nodiscard]] auto getMessage() const -> std::filesystem::path;
	void sendMessage(const std::string_view &message);

  private:
	rtos::EventFlags &_event_flags;

	mbed::BufferedSerial _serial {CONSOLE_TX, CONSOLE_RX, 115200};

	std::string _reception_string;
};
