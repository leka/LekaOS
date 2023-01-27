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

class FileTestFlush : public ::testing::Test
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

TEST_F(FileTestFlush, flushNoFile)
{
	auto flush = file.flush();

	ASSERT_FALSE(flush);
}

TEST_F(FileTestFlush, flush)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w+");

	file.write(input_data);

	auto flush = file.flush();

	auto output_data = readTempFile();

	ASSERT_EQ("abcdef", output_data);
}

TEST_F(FileTestFlush, setBufferThenFlush)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w+");

	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_TRUE(buffering);

	file.write(input_data);

	auto flush = file.flush();

	auto output_data = readTempFile();

	ASSERT_EQ("abcdef", output_data);
}

TEST_F(FileTestFlush, setBufferNoFlushThenFlush)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w+");

	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_TRUE(buffering);

	file.write(input_data);

	auto output_data_no_flush = readTempFile();

	ASSERT_EQ("", output_data_no_flush);

	auto flush = file.flush();

	auto output_data_flush = readTempFile();

	ASSERT_EQ("abcdef", output_data_flush);
}
