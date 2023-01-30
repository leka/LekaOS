// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>
#include <span>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestRead : public ::testing::Test
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

TEST_F(FileTestRead, readSpan)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTestRead, readSpanBinary)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTestRead, readCharSpan)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<char, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTestRead, readCharSpanBinary)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	auto output_data = std::array<char, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTestRead, readBufferAndSize)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	uint8_t output_data[6];

	auto size = file.read(output_data, std::size(output_data));

	ASSERT_EQ(std::size(output_data), size);
	for (uint32_t i = 0; i < size; ++i) {
		ASSERT_EQ(input_data[i], output_data[i]);
	}
}

TEST_F(FileTestRead, readBufferAndSizeBinary)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	uint8_t output_data[6];

	auto size = file.read(output_data, std::size(output_data));

	ASSERT_EQ(std::size(output_data), size);
	for (uint32_t i = 0; i < size; ++i) {
		ASSERT_EQ(input_data[i], output_data[i]);
	}
}

TEST_F(FileTestRead, readCharBufferAndSize)
{
	char input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};	// "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	char output_data[6];

	auto size = file.read(output_data, std::size(output_data));

	ASSERT_EQ(std::size(output_data), size);
	for (uint32_t i = 0; i < size; ++i) {
		ASSERT_EQ(input_data[i], output_data[i]);
	}
}

TEST_F(FileTestRead, readCharBufferAndSizeBinary)
{
	char input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};	// "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	char output_data[6];

	auto size = file.read(output_data, std::size(output_data));

	ASSERT_EQ(std::size(output_data), size);
	for (uint32_t i = 0; i < size; ++i) {
		ASSERT_EQ(input_data[i], output_data[i]);
	}
}

TEST_F(FileTestRead, writeThenRead)
{
	auto input_data	 = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"
	auto output_data = std::array<uint8_t, 6> {};

	file.open(tempFilename, "w+");

	auto bytes_written = file.write(input_data);
	file.rewind();
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(bytes_written, bytes_read);
	ASSERT_EQ(input_data, output_data);
}
