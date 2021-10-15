// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_PLATFORM_FILE_MOCK_H_
#define _LEKA_OS_PLATFORM_FILE_MOCK_H_

#include "gmock/gmock.h"
#include "interface/platform/File.h"

namespace leka::mock {

class File : public interface::File
{
  public:
	MOCK_METHOD(bool, open, (const char *, const char *), (override));

	MOCK_METHOD(void, close, (), (override));

	MOCK_METHOD(size_t, read, (lstd::span<uint8_t>), (override));
	MOCK_METHOD(size_t, write, (lstd::span<uint8_t>), (override));

	MOCK_METHOD(size_t, read, (uint8_t *, uint32_t), (override));
	MOCK_METHOD(size_t, write, (uint8_t *, uint32_t), (override));

	MOCK_METHOD(void, seek, (size_t, int), (override));

	MOCK_METHOD(void, rewind, (), (override));

	MOCK_METHOD(size_t, size, (), (override));

	MOCK_METHOD(bool, is_open, (), (const override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_PLATFORM_FILE_MOCK_H_
