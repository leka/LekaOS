// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/BufferedSerial.h"

#include "interface/drivers/BufferedSerial.h"

namespace leka {

class CoreBufferedSerial : public interface::BufferedSerial
{
  public:
	explicit CoreBufferedSerial(PinName tx, PinName rx, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
		: _serial(tx, rx, baud)
	{
	}

	auto read(uint8_t *buffer, std::size_t length) -> std::size_t final;
	auto write(const uint8_t *buffer, std::size_t length) -> std::size_t final;

	auto readable() -> bool final;

	void enable_input() final;
	void disable_input() final;

	void sigio(std::function<void()> const &callback) final;

  private:
	mbed::BufferedSerial _serial;

	std::function<void()> _sigio_callback {};
};

}	// namespace leka
