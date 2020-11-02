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

	uint32_t getFileSize(const char *filename);

	void testWorkingToRemove();

  private:
	SDBlockDevice _bd;
	FATFileSystem _fs;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILEMANAGER_H_
