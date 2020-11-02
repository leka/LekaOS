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
	void showRootDirectory();

  private:
	SDBlockDevice _block_interface;
	FATFileSystem _file_interface;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILEMANAGER_H_
