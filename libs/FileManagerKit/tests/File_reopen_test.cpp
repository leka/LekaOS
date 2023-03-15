// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>
#include <fstream>
#include <string>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestReopen : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(tempFilename, "/tmp/XXXXXX");
		mkstemp(tempFilename);
		tempFilename_filesystem_path = tempFilename;
	}
	// void TearDown() override {}

	auto readTempFile() -> std::string
	{
		std::fstream f {};
		f.open(tempFilename);

		std::stringstream out {};
		out << f.rdbuf();
		f.close();

		return out.str();
	}

	FileManagerKit::File file {};
	char tempFilename[L_tmpnam];   // NOLINT
	std::filesystem::path tempFilename_filesystem_path;
};

TEST_F(FileTestReopen, reopenNoFile)
{
	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTestReopen, openThenReopenFile)
{
	file.open(tempFilename, "r");

	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_TRUE(reopen);
}

TEST_F(FileTestReopen, reopenNoFileWithFileSystemPath)
{
	auto reopen = file.reopen(tempFilename_filesystem_path, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTestReopen, openThenReopenFileWithFileSystemPath)
{
	file.open(tempFilename_filesystem_path, "r");

	auto reopen = file.reopen(tempFilename_filesystem_path, "w");

	ASSERT_TRUE(reopen);
}

TEST_F(FileTestReopen, closeThenReopenFile)
{
	file.open(tempFilename, "r");

	file.close();

	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTestReopen, reopen)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "r");
	file.reopen(tempFilename, "w");
	file.write(input_data);
	file.close();

	auto output_data_w = readTempFile();

	ASSERT_EQ("abcdef", output_data_w);
}
