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
	explicit CoreBufferedSerial(mbed::BufferedSerial &serial) : _serial(serial) {}

	auto read(uint8_t *buffer, ssize_t length) -> ssize_t final;
	auto write(const uint8_t *buffer, ssize_t length) -> ssize_t final;
	auto readable() -> bool final;

  private:
	mbed::BufferedSerial &_serial;
};

}	// namespace leka
