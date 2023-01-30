// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestSeek : public ::testing::Test
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

TEST_F(FileTestSeek, seek)
{
	auto input_data	   = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	auto output_data   = std::array<uint8_t, 6> {};
	auto expected_data = std::array<uint8_t, 6> {0x64, 0x65, 0x66, 0x00, 0x00, 0x00};

	file.open(tempFilename, "w+");

	std::ignore = file.write(input_data);
	file.seek(3);
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(3, bytes_read);
	ASSERT_EQ(expected_data, output_data);
}
