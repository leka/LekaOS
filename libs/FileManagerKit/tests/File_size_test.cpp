// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestSize : public ::testing::Test
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
	std::filesystem::path tempFilename_filesystem_path;
};

TEST_F(FileTestSize, sizeNoFile)
{
	auto size = file.size();
	ASSERT_EQ(0, size);
}

TEST_F(FileTestSize, sizeEmptyFile)
{
	file.open(tempFilename);
	auto size = file.size();
	ASSERT_EQ(0, size);
}

TEST_F(FileTestSize, sizeFile)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename);
	auto expected_size = file.write(input_data);

	auto actual_size = file.size();

	ASSERT_EQ(expected_size, actual_size);
}
