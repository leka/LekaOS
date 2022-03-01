// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/FatFs.h"

namespace leka {

class CoreFatFs : public interface::FatFs
{
  public:
	CoreFatFs() = default;	 // SDBlockDevice must be initialized and mounted before using CoreFatFs

	auto open(const char *path) -> FRESULT final;
	auto close() -> FRESULT final;
	auto read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read) -> FRESULT final;
	auto write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written)
		-> FRESULT final;
	auto seek(uint32_t byte_offset_from_top_of_file) -> FRESULT final;
	auto getSize() -> uint32_t final;

	auto getPointer() -> FIL * final;

  private:
	FIL _file {};
};

}	// namespace leka
