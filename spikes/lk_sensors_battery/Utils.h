// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_UTILS_H_
#define _LEKA_OS_SPIKE_UTILS_H_

#include <cstdint>
#include <memory>

#include "FileManager.h"
#include "LogKit.h"

auto readSdFile(const char *fileName, char *buffer, uint32_t buffer_size) -> int
{
	auto file = std::make_unique<FIL>();
	UINT nbBytesRead {0};

	if (f_open(file.get(), fileName, FA_READ | FA_OPEN_ALWAYS) != FR_OK) {
		log_error("Failed to open");
		return -1;
	}
	f_lseek(file.get(), 0);	  // Set the pointer of the file at the beginning. Otherwise it will appear blank to read
							  // since the pointer is at the end of the file.

	if (f_read(file.get(), reinterpret_cast<BYTE *>(buffer), buffer_size, &nbBytesRead) != FR_OK) {
		log_error("Failed to read");
		return -1;
	}
	if (f_close(file.get()) != FR_OK) {
		log_error("Failed to close");
		return -1;
	}

	return static_cast<int>(nbBytesRead);
}

auto writeSDFile(const char *fileName, const char *data, int nbByteToWrite) -> int
{
	auto file = std::make_unique<FIL>();
	UINT nbBytesWritten {0};

	if (f_open(file.get(), fileName, FA_READ | FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
		log_error("Failed to open");
		return -1;
	}

	if (f_write(file.get(), data, nbByteToWrite, &nbBytesWritten) != FR_OK) {
		log_error("Failed to Write");
		return -1;
	}

	if (f_close(file.get()) != FR_OK) {
		log_error("Failed to close");
		return -1;
	}

	return static_cast<int>(nbBytesWritten);
}

#endif	 // _LEKA_OS_SPIKE_UTILS_H_
