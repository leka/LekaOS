// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <fstream>
#include <iostream>
#include <span>
#include <string>

#include "FileManagerKit.h"
#include "LogKit.h"
#include "filesystem"
#include "gtest/gtest.h"

using namespace leka;

class FileTest : public ::testing::Test
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

TEST_F(FileTest, errorNoFile)
{
	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTest, errorWriting)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "w");

	file.write(input_data);

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTest, errorWritingModeRead)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "r");

	file.write(input_data);

	auto error = file.error();

	ASSERT_TRUE(error);
}

TEST_F(FileTest, errorReading)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<uint8_t, 6> {};

	file.read(output_data);

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTest, errorReadingModeWrite)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "w");

	auto output_data = std::array<uint8_t, 6> {};

	file.read(output_data);

	auto error = file.error();

	ASSERT_TRUE(error);
}

TEST_F(FileTest, clearErrorNoFile)
{
	file.clearerr();

	auto error = file.error();

	ASSERT_FALSE(error);
}

TEST_F(FileTest, clearError)
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

TEST_F(FileTest, clear)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename);
	auto size = file.size();
	EXPECT_NE(size, 0);

	file.clear();

	size = file.size();
	EXPECT_EQ(size, 0);

	file.close();
}

TEST_F(FileTest, clearNotExistingFile)
{
	ASSERT_FALSE(file.is_open());

	file.clear();

	// nothing expected
}
