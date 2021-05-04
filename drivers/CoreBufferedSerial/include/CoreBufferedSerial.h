// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_
#define _LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_

// ? LCOV_EXCL_START - Exclude from coverage report

#include "drivers/BufferedSerial.h"

namespace leka::interface {

class BufferedSerial
{
  public:
	virtual ~BufferedSerial() = default;

	virtual auto read(uint8_t *buffer, ssize_t length) -> ssize_t		 = 0;
	virtual auto write(const uint8_t *buffer, ssize_t length) -> ssize_t = 0;
};

}	// namespace leka::interface

namespace leka {

class CoreBufferedSerial : public interface::BufferedSerial
{
  public:
	explicit CoreBufferedSerial(mbed::BufferedSerial &serial) : _serial {serial} {};

	auto read(uint8_t *buffer, ssize_t length) -> ssize_t final;
	auto write(const uint8_t *buffer, ssize_t length) -> ssize_t final;

  private:
	mbed::BufferedSerial &_serial;
};

}	// namespace leka

// ? LCOV_EXCL_STOP - Exclude from coverage report

#endif	 //_LEKA_OS_DRIVER_CORE_BUFFERED_SERIAL_H_
