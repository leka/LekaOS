// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileManager.h"

namespace leka {

using namespace mbed;

FileManager::FileManager() : _bd(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK), _fs("fs")
{
	_bd.init();
	_bd.frequency(5000000);

	_fs.mount(&_bd);
}

uint32_t FileManager::getFileSize(const char *filename)
{
	FIL file;
	uint32_t file_size = 0;

	if (f_open(&file, filename, FA_READ) == FR_OK) {
		file_size = f_size(&file);

		f_close(&file);
	}

	return file_size;
}

void FileManager::testWorkingToRemove()
{
	DIR *dir;
	struct dirent *p;

	dir = opendir("/fs");
	if (dir != NULL) {
		printf("At root of SD card:\n");
		while ((p = readdir(dir)) != NULL) {
			printf(" - %s\n", p->d_name);
		}
	} else {
		printf("Could not open directory!\n");
	}
	closedir(dir);

	return;
}

}	// namespace leka
