// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreFatFsBase.h"
#include "gmock/gmock.h"

namespace leka {

class CoreFatFsMock : public interface::CoreFatFsBase
{
  public:
	MOCK_METHOD(FRESULT, open, (const char *path), (override));
	MOCK_METHOD(FRESULT, close, (), (override));
	MOCK_METHOD(FRESULT, read, (void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read),
				(override));
	MOCK_METHOD(FRESULT, write,
				(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written), (override));
	MOCK_METHOD(FRESULT, seek, (uint32_t bytes_offset_from_top_of_file), (override));
	MOCK_METHOD(uint32_t, getSize, (), (override));
	MOCK_METHOD(FIL *, getPointer, (), (override));
};

}	// namespace leka
