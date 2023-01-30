// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestOpenClose : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(tempFilename, "/tmp/XXXXXX");
		mkstemp(tempFilename);
		tempFilename_filesystem_path = tempFilename;
	}
	// void TearDown() override {}

	FileManagerKit::File file {};
	char tempFilename[L_tmpnam];   // NOLINT
	std::filesystem::path tempFilename_filesystem_path {};
};

TEST_F(FileTestOpenClose, open)
{
	file.open(tempFilename);

	ASSERT_TRUE(file.is_open());
}

TEST_F(FileTestOpenClose, openByFileSystemPath)
{
	file.open(tempFilename_filesystem_path);

	ASSERT_TRUE(file.is_open());
}

TEST_F(FileTestOpenClose, close)
{
	file.open(tempFilename);
	file.close();

	ASSERT_FALSE(file.is_open());
}

TEST_F(FileTestOpenClose, closeNoFile)
{
	file.close();

	ASSERT_FALSE(file.is_open());
}
