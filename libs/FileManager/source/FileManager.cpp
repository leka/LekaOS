// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileManager.h"

#include "LogKit.h"

namespace leka {

namespace {
	constexpr auto default_bd_frequency = uint64_t {25'000'000};
}

using namespace mbed;

// TODO (@yann) - _file is not initialized - make it a pointer?
FileManager::FileManager() : _bd(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK), _fs("fs")
{
	_bd.init();
	_bd.frequency(default_bd_frequency);

	_fs.mount(&_bd);
}

auto FileManager::open(const char *filename) -> bool
{
	bool file_opened = false;

	if (f_open(&_file, filename, FA_READ) == FR_OK) {
		file_opened = true;
	}

	return file_opened;
}

auto FileManager::close() -> bool
{
	bool file_closed = false;

	if (f_close(&_file) == FR_OK) {
		file_closed = true;
	}

	return file_closed;
}

auto FileManager::write(const char *data, const uint32_t size) -> bool
{
	return f_write(&_file, data, static_cast<UINT>(size), nullptr) == FR_OK;
}

auto FileManager::getFileSize() -> uint32_t
{
	uint32_t file_size = 0;

	if (_file.obj.fs != nullptr) {
		file_size = f_size(&_file);
	}

	return file_size;
}

// TODO (@yann) - remove if not needed + remove include LogKit
void FileManager::testWorkingToRemove()
{
	DIR *dir = opendir("/fs");

	if (dir != nullptr) {
		struct dirent *p;
		log_debug("At root of SD card:");
		while ((p = readdir(dir)) != nullptr) {
			log_debug("\t- %s", p->d_name);
		}
	} else {
		log_error("Could not open directory!");
	}

	closedir(dir);
}

}	// namespace leka
