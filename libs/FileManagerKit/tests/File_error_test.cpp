// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>
#include <span>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestError : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(tempFilename, "/tmp/XXXXXX");
		mkstemp(tempFilename);
		tempFilename_filesystem_path = tempFilename;
	}
	// void TearDown() override {}

	void writeTempFile(std::span<uint8_t> data)
	{
		auto *file = fopen(tempFilename, "wb");
		fwrite(data.data(), sizeof(uint8_t), data.size(), file);
		fclose(file);
	}

	void writeTempFile(std::span<char> data)
	{
		auto *file = fopen(tempFilename, "w");
		fwrite(data.data(), sizeof(char), data.size(), file);
		fclose(file);
	}

	FileManagerKit::File file {};
	char tempFilename[L_tmpnam];   // NOLINT
	std::filesystem::path tempFilename_filesystem_path;
};

TEST_F(FileTestError, errorNoFile)
{
	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTestError, errorWriting)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "w");

	file.write(input_data);

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTestError, errorWritingModeRead)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "r");

	file.write(input_data);

	auto error = file.error();

	ASSERT_TRUE(error);
}

TEST_F(FileTestError, errorReading)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<uint8_t, 6> {};

	file.read(output_data);

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTestError, errorReadingModeWrite)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "w");

	auto output_data = std::array<uint8_t, 6> {};

	file.read(output_data);

	auto error = file.error();

	ASSERT_TRUE(error);
}

TEST_F(FileTestError, clearErrorNoFile)
{
	file.clearerr();

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTestError, clearError)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "r");

	file.write(input_data);

	auto error = file.error();

	ASSERT_TRUE(error);

	file.clearerr();

	auto error_after_clear = file.error();

	ASSERT_FALSE(error_after_clear);
}
