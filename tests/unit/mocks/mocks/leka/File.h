// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <span>

#include "filesystem"
#include "gmock/gmock.h"
#include "interface/platform/File.h"

namespace leka::mock {

class File : public interface::File
{
  public:
	MOCK_METHOD(bool, open, (const char *, const char *), (override));
	MOCK_METHOD(bool, open, (const std::filesystem::path &, const char *), (override));

	MOCK_METHOD(void, close, (), (override));

	MOCK_METHOD(size_t, read, (std::span<uint8_t>), (override));
	MOCK_METHOD(size_t, write, (std::span<uint8_t>), (override));

	MOCK_METHOD(size_t, read, (std::span<char>), (override));
	MOCK_METHOD(size_t, write, (std::span<char>), (override));

	MOCK_METHOD(size_t, read, (std::span<int16_t>), (override));

	MOCK_METHOD(size_t, read, (uint8_t *, uint32_t), (override));
	MOCK_METHOD(size_t, write, (const uint8_t *, uint32_t), (override));

	MOCK_METHOD(size_t, read, (char *, uint32_t), (override));
	MOCK_METHOD(size_t, write, (const char *, uint32_t), (override));

	MOCK_METHOD(void, seek, (size_t, int), (override));

	MOCK_METHOD(void, clear, (), (override));
	MOCK_METHOD(void, rewind, (), (override));

	MOCK_METHOD(size_t, size, (), (override));

	MOCK_METHOD(size_t, tell, (), (override));

	MOCK_METHOD(bool, reopen, (const char *, const char *), (override));
	MOCK_METHOD(bool, reopen, (const std::filesystem::path &, const char *), (override));

	MOCK_METHOD(bool, setBuffer, (std::span<char> buffer, int mode), (override));

	MOCK_METHOD(bool, setBuffer, (char *buffer, uint32_t size, int mode), (override));

	MOCK_METHOD(bool, unsetBuffer, (), (override));

	MOCK_METHOD(bool, flush, (), (override));

	MOCK_METHOD(bool, error, (), (override));

	MOCK_METHOD(void, clearerr, (), (override));

	MOCK_METHOD(bool, is_open, (), (const override));

	MOCK_METHOD((std::array<uint8_t, 32>), getSHA256, (), (override));
};

}	// namespace leka::mock
