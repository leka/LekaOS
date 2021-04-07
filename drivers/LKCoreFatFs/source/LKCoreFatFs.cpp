// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreFatFs.h"

namespace leka {

FRESULT LKCoreFatFs::open(const char *path)
{
	// TODO: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r1.html
	return f_open(&_file, path, FA_READ);
}

FRESULT LKCoreFatFs::close()
{
	return f_close(&_file);
}

FRESULT LKCoreFatFs::read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read)
{
	return f_read(&_file, buffer, number_of_bytes_to_read, reinterpret_cast<UINT *>(number_of_bytes_read));
}

FRESULT LKCoreFatFs::write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written)
{
	return f_write(&_file, buffer, number_of_bytes_to_write, reinterpret_cast<UINT *>(number_of_bytes_written));
}

FRESULT LKCoreFatFs::seek(uint32_t byte_offset_from_top_of_file)
{
	return f_lseek(&_file, byte_offset_from_top_of_file);
}

uint32_t LKCoreFatFs::getSize()
{
	return f_size(&_file);
}

FIL *LKCoreFatFs::getPointer()
{
	return &_file;
}

}	// namespace leka
