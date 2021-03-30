// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_UTILS_H_
#define _LEKA_OS_SPIKE_UTILS_H_

#include <cstdint>

#include "FileManager.h"

int readSdFile(const char *fileName, char *buffer, uint32_t buffer_size)
{
	FIL file;
	UINT nbBytesRead {0};

	if (f_open(&file, fileName, FA_READ | FA_OPEN_ALWAYS) != FR_OK) {
		printf("Failed to open.\n");
		return -1;
	}
	f_lseek(&file, 0);	 // Set the pointer of the file at the beginning. Otherwise it will appear blank to read since
						 // the pointer is at the end of the file.

	if (f_read(&file, reinterpret_cast<BYTE *>(buffer), buffer_size, &nbBytesRead) != FR_OK) {
		printf("Failed to read\n");
		return -1;
	}
	if (f_close(&file) != FR_OK) {
		printf("Failed to close.\n");
		return -1;
	}
	return static_cast<int>(nbBytesRead);
}

int writeSDFile(const char *fileName, const char *data, int nbByteToWrite)
{
	FIL file;
	UINT nbBytesWritten {0};

	if (f_open(&file, fileName, FA_READ | FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
		printf("Failed to open.\n");
		return -1;
	}

	if (f_write(&file, data, nbByteToWrite, &nbBytesWritten) != FR_OK) {
		printf("Failed to Write.\n");
		return -1;
	}

	if (f_close(&file) != FR_OK) {
		printf("Failed to close.\n");
		return -1;
	}
	return static_cast<int>(nbBytesWritten);
}

#endif	 // _LEKA_OS_SPIKE_UTILS_H_
