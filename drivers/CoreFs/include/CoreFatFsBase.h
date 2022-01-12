// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FATFS_BASE_H_
#define _LEKA_OS_LIB_FATFS_BASE_H_

#include <cstdint>

#include "storage/filesystem/fat/ChaN/ff.h"

namespace leka {

class CoreFatFsBase
{
  public:
	virtual ~CoreFatFsBase() = default;

	virtual FRESULT open(const char *path)																			= 0;
	virtual FRESULT close()																							= 0;
	virtual FRESULT read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read)			= 0;
	virtual FRESULT write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written) = 0;
	virtual FRESULT seek(uint32_t bytes_offset_from_top_of_file)													= 0;
	virtual uint32_t getSize()																						= 0;

	virtual FIL *getPointer() = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FATFS_BASE_H_
