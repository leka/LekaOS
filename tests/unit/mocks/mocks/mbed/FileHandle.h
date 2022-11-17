// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "PinNames.h"

#include "platform/FileHandle.h"

#include "gmock/gmock.h"

namespace mbed::mock {

class FileHandle : public mbed::FileHandle
{
  public:
	MOCK_METHOD(ssize_t, read, (void *buffer, std::size_t size), (override));
	MOCK_METHOD(ssize_t, write, (const void *buffer, std::size_t size), (override));
	MOCK_METHOD(off_t, seek, (off_t offset, int whence), (override));
	MOCK_METHOD(int, close, (), (override));
};

}	// namespace mbed::mock
