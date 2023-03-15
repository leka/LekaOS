// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestTell : public ::testing::Test
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

TEST_F(FileTestTell, tellNoFile)
{
	auto pos = file.tell();
	ASSERT_EQ(0, pos);
}

TEST_F(FileTestTell, tellEmptyFile)
{
	file.open(tempFilename);
	auto pos = file.tell();
	ASSERT_EQ(0, pos);
}

TEST_F(FileTestTell, tellFile)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "w");
	file.write(input_data);
	file.seek(3);

	auto actual_pos = file.tell();

	ASSERT_EQ(3, actual_pos);
}
