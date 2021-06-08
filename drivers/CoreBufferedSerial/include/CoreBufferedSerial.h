// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_
#define _LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_

#include "drivers/BufferedSerial.h"

#include "interface/drivers/BufferedSerial.h"

namespace leka {

class CoreBufferedSerial : public interface::BufferedSerial
{
  public:
	explicit CoreBufferedSerial(mbed::BufferedSerial &serial) : _serial {serial} {};

	auto read(uint8_t *buffer, ssize_t length) -> ssize_t final;
	auto write(const uint8_t *buffer, ssize_t length) -> ssize_t final;
	auto readable() -> bool final;
	void registerIOCallback(mbed::Callback<void()> func) final;

  private:
	mbed::BufferedSerial &_serial;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_
