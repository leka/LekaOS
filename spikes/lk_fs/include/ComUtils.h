// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <string>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/EventFlags.h"

#include "CoreEventQueue.h"

class ComUtils
{
  public:
	explicit ComUtils(rtos::EventFlags &event_flags);

	void read();

	void write(std::string_view message);

	[[nodiscard]] auto getPath() const -> std::filesystem::path;

	struct flags {
		static constexpr uint16_t data_available = 1UL << 1;
	};

  private:
	rtos::EventFlags &_event_flags;

	leka::CoreEventQueue _event_queue {};

	mbed::BufferedSerial _serial {CONSOLE_TX, CONSOLE_RX, 115200};

	std::string _buffer;
};
