// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// DEPRECATED - Use FATFileSystem and CoreFatFs instead

#ifndef _LEKA_OS_LIB_FILEMANAGER_H_
#define _LEKA_OS_LIB_FILEMANAGER_H_

#include "FATFileSystem.h"
#include "SDBlockDevice.h"

namespace leka {

// TODO (@yann) - move the bd and fs part to FileSystemKit
class FileManager
{
  public:
	FileManager();

	auto open(const char *filename) -> bool;
	auto close() -> bool;
	auto write(const char *data, uint32_t size) -> bool;
	auto getFileSize() -> uint32_t;

	void testWorkingToRemove();

  private:
	SDBlockDevice _bd;
	FATFileSystem _fs;
	FIL _file {};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILEMANAGER_H_
