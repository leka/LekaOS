// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MBED_MOCK_FILE_HANDLE_H_
#define _LEKA_OS_MBED_MOCK_FILE_HANDLE_H_

#include "PinNames.h"

#include "platform/FileHandle.h"

#include "gmock/gmock.h"

namespace mbed::mock {

class FileHandle : public mbed::FileHandle
{
  public:
	MOCK_METHOD(ssize_t, read, (void *buffer, size_t size), (override));
	MOCK_METHOD(ssize_t, write, (const void *buffer, size_t size), (override));
	MOCK_METHOD(off_t, seek, (off_t offset, int whence), (override));
	MOCK_METHOD(int, close, (), (override));
};

}	// namespace mbed::mock

#endif	 // _LEKA_OS_MBED_MOCK_FILE_HANDLE_H_
