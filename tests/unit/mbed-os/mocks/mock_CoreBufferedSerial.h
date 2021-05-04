// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BUFFERED_SERIAL_MOCK_H_
#define _LEKA_OS_BUFFERED_SERIAL_MOCK_H_

#include "CoreBufferedSerial.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreBufferedSerialMock : public interface::BufferedSerial
{
  public:
	MOCK_METHOD(int, read, (void *, ssize_t), (override));
	MOCK_METHOD(int, write, (const void *, ssize_t), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_BUFFERED_SERIAL_MOCK_H_
