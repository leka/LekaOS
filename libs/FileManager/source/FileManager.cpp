// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileManager.h"

namespace leka {

using namespace mbed;

FileManager::FileManager() : _bd(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK), _fs("fs")
{
	_bd.init();
	_bd.frequency(25000000);

	_fs.mount(&_bd);
}

bool FileManager::open(const char *filename)
{
	bool file_opened = false;

	if (f_open(&_file, filename, FA_READ) == FR_OK) {
		file_opened = true;
	}

	return file_opened;
}

bool FileManager::close()
{
	bool file_closed = false;

	if (f_close(&_file) == FR_OK) {
		file_closed = true;
	}

	return file_closed;
}

uint32_t FileManager::getFileSize()
{
	uint32_t file_size = 0;

	if (_file.obj.fs != NULL) {
		file_size = f_size(&_file);
	}

	return file_size;
}

void FileManager::testWorkingToRemove()
{
	DIR *dir = opendir("/fs");

	if (dir != NULL) {
		struct dirent *p;
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
