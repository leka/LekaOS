// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

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

	FATFileSystem _fs;

  private:
	SDBlockDevice _bd;
	FIL _file;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILEMANAGER_H_
