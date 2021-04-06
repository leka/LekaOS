// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FATFS_H_
#define _LEKA_OS_LIB_FATFS_H_

#include "LKCoreFatFsBase.h"

namespace leka {

class LKCoreFatFs : public LKCoreFatFsBase
{
  public:
	LKCoreFatFs() = default;   // SDBlockDevice must be initialized and mounted before using LKCoreFatFs

	FRESULT open(const char *path) final;
	FRESULT close() final;
	FRESULT read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read) final;
	FRESULT write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written) final;
	FRESULT seek(uint32_t bytes_offset_from_top_of_file) final;
	uint32_t getSize() final;

	FIL *getPointer() final;

  private:
	FIL _file;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FATFS_H_
