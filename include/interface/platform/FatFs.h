// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef __PLATFORM_HEADER__
#define __PLATFORM_HEADER__

#include <cstdint>

#include "storage/filesystem/fat/ChaN/ff.h"

namespace leka::interface {

// TODO (@ladislas) - return int, bool or std::optional instead of FRESULT
//  test comment for CI
class FatFs
{
  public:
	virtual ~FatFs() = default;

	virtual auto open(const char *path) -> FRESULT																 = 0;
	virtual auto close() -> FRESULT																				 = 0;
	virtual auto read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read) -> FRESULT = 0;
	virtual auto write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written)
		-> FRESULT														= 0;
	virtual auto seek(uint32_t byte_offset_from_top_of_file) -> FRESULT = 0;
	virtual auto getSize() -> uint32_t									= 0;

	virtual auto getPointer() -> FIL * = 0;
};

}	// namespace leka::interface

#endif
