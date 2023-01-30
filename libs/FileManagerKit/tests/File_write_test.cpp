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

class FileTestWrite : public ::testing::Test
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

TEST_F(FileTestWrite, writeSpan)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeSpanBinary)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "wb");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeCharSpan)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeCharSpanBinary)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	file.open(tempFilename, "wb");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeBufferAndSize)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	file.open(tempFilename, "w");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeBufferAndSizeBinary)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	file.open(tempFilename, "wb");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeCharBufferAndSize)
{
	char input_data[] = {'a', 'b', 'c', 'd', 'e', 'f'};

	file.open(tempFilename, "w");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTestWrite, writeCharBufferAndSizeBinary)
{
	char input_data[] = {'a', 'b', 'c', 'd', 'e', 'f'};

	file.open(tempFilename, "wb");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}
