// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFatFs.h"

namespace leka {

auto CoreFatFs::open(const char *path) -> FRESULT
{
	return f_open(&_file, path, FA_READ);
}

auto CoreFatFs::close() -> FRESULT
{
	return f_close(&_file);
}

auto CoreFatFs::read(void *buffer, uint32_t number_of_bytes_to_read, uint32_t *number_of_bytes_read) -> FRESULT
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	return f_read(&_file, buffer, number_of_bytes_to_read, reinterpret_cast<UINT *>(number_of_bytes_read));
}

auto CoreFatFs::write(const void *buffer, uint32_t number_of_bytes_to_write, uint32_t *number_of_bytes_written)
	-> FRESULT
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	return f_write(&_file, buffer, number_of_bytes_to_write, reinterpret_cast<UINT *>(number_of_bytes_written));
}

auto CoreFatFs::seek(uint32_t byte_offset_from_top_of_file) -> FRESULT
{
	return f_lseek(&_file, byte_offset_from_top_of_file);
}

auto CoreFatFs::getSize() -> uint32_t
{
	return f_size(&_file);
}

auto CoreFatFs::getPointer() -> FIL *
{
	return &_file;
}

}	// namespace leka
