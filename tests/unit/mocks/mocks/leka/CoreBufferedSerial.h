// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BUFFERED_SERIAL_MOCK_H_
#define _LEKA_OS_BUFFERED_SERIAL_MOCK_H_

#include "CoreBufferedSerial.h"
#include "gmock/gmock.h"

namespace leka {

class CoreBufferedSerialMock : public interface::BufferedSerial
{
  public:
	MOCK_METHOD(ssize_t, read, (uint8_t *, ssize_t), (override));
	MOCK_METHOD(ssize_t, write, (const uint8_t *, ssize_t), (override));
	MOCK_METHOD(bool, readable, (), (override));
	MOCK_METHOD(void, registerIOCallback, (mbed::Callback<void()> func), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_BUFFERED_SERIAL_MOCK_H_
