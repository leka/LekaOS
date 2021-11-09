// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <fstream>
#include <iostream>
#include <lstd_array>
#include <lstd_span>
#include <string>

#include "FileSystemKit.h"
#include "LogKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(tempFilename, "/tmp/XXXXXX");
		mkstemp(tempFilename);
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

	void writeTempFile(lstd::span<uint8_t> data)
	{
		auto *file = fopen(tempFilename, "wb");
		fwrite(data.data(), sizeof(uint8_t), data.size(), file);
		fclose(file);
	}

	void writeTempFile(lstd::span<char> data)
	{
		auto *file = fopen(tempFilename, "w");
		fwrite(data.data(), sizeof(char), data.size(), file);
		fclose(file);
	}

	FileSystemKit::File file {};
	char tempFilename[L_tmpnam];   // NOLINT
};

TEST_F(FileTest, initializationDefault)
{
	auto new_file = FileSystemKit::File {};
	ASSERT_NE(&new_file, nullptr);
	ASSERT_FALSE(new_file.is_open());
}

TEST_F(FileTest, initializationWithPath)
{
	auto new_file = FileSystemKit::File {tempFilename};
	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTest, initializationWithPathAndMode)
{
	auto new_file = FileSystemKit::File {tempFilename, "r"};

	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTest, initializationWithNullptr)
{
	{
		auto f = FileSystemKit::File {"tempFilename", "r"};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileSystemKit::File {nullptr, nullptr};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileSystemKit::File {"nullptr", nullptr};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileSystemKit::File {nullptr, "r"};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileSystemKit::File {nullptr};
		ASSERT_FALSE(f.is_open());
	}
}

TEST_F(FileTest, open)
{
	file.open(tempFilename);

	ASSERT_TRUE(file.is_open());
}

TEST_F(FileTest, close)
{
	file.open(tempFilename);
	file.close();

	ASSERT_FALSE(file.is_open());
}

TEST_F(FileTest, closeNoFile)
{
	file.close();

	ASSERT_FALSE(file.is_open());
}

TEST_F(FileTest, writeSpan)
{
	auto input_data = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeSpanBinary)
{
	auto input_data = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "wb");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharSpan)
{
	auto input_data = lstd::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharSpanBinary)
{
	auto input_data = lstd::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	file.open(tempFilename, "wb");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeBufferAndSize)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	file.open(tempFilename, "w");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeBufferAndSizeBinary)
{
	uint8_t input_data[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};   // "abcdef"

	file.open(tempFilename, "wb");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharBufferAndSize)
{
	char input_data[] = {'a', 'b', 'c', 'd', 'e', 'f'};

	file.open(tempFilename, "w");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharBufferAndSizeBinary)
{
	char input_data[] = {'a', 'b', 'c', 'd', 'e', 'f'};

	file.open(tempFilename, "wb");
	auto size = file.write(input_data, std::size(input_data));

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, readSpan)
{
	auto input_data = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readSpanBinary)
{
	auto input_data = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readCharSpan)
{
	auto input_data = lstd::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<char, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readCharSpanBinary)
{
	auto input_data = lstd::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	auto output_data = std::array<char, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readBufferAndSize)
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

TEST_F(FileTest, readBufferAndSizeBinary)
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

TEST_F(FileTest, readCharBufferAndSize)
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

TEST_F(FileTest, readCharBufferAndSizeBinary)
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

TEST_F(FileTest, sizeNoFile)
{
	auto size = file.size();
	ASSERT_EQ(0, size);
}

TEST_F(FileTest, sizeEmptyFile)
{
	file.open(tempFilename);
	auto size = file.size();
	ASSERT_EQ(0, size);
}

TEST_F(FileTest, sizeFile)
{
	auto input_data = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename);
	auto expected_size = file.write(input_data);

	auto actual_size = file.size();

	ASSERT_EQ(expected_size, actual_size);
}

TEST_F(FileTest, writeThenRead)
{
	auto input_data	 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	auto output_data = std::array<uint8_t, 6> {};

	file.open(tempFilename, "w+");

	auto bytes_written = file.write(input_data);
	file.rewind();
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(bytes_written, bytes_read);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, seek)
{
	auto input_data	   = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"
	auto output_data   = std::array<uint8_t, 6> {};
	auto expected_data = std::array<uint8_t, 6> {0x64, 0x65, 0x66, 0x00, 0x00, 0x00};

	file.open(tempFilename, "w+");

	auto _ = file.write(input_data);
	file.seek(3);
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(3, bytes_read);
	ASSERT_EQ(expected_data, output_data);
}
