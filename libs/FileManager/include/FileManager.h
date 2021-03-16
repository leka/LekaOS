// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// DEPRECATED - Use FATFileSystem and LKCoreFatFs instead

#ifndef _LEKA_OS_LIB_FILEMANAGER_H_
#define _LEKA_OS_LIB_FILEMANAGER_H_

#include "FATFileSystem.h"
#include "SDBlockDevice.h"

namespace leka {

class FileManager
{
  public:
	FileManager();

	bool open(const char *filename);
	bool close();
	uint32_t getFileSize();

	void testWorkingToRemove();

  private:
	SDBlockDevice _bd;
	FATFileSystem _fs;
	FIL _file;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILEMANAGER_H_
