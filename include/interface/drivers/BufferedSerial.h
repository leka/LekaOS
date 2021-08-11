// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_BUFFERED_SERIAL_H_
#define _LEKA_OS_INTERFACE_DRIVER_BUFFERED_SERIAL_H_

#include "mbed.h"

namespace leka::interface {

class BufferedSerial
{
  public:
	virtual ~BufferedSerial() = default;

	virtual auto read(uint8_t *buffer, ssize_t length) -> ssize_t		 = 0;
	virtual auto write(const uint8_t *buffer, ssize_t length) -> ssize_t = 0;
	virtual auto readable() -> bool										 = 0;
	virtual void registerIOCallback(mbed::Callback<void()> func)		 = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_BUFFERED_SERIAL_H_
