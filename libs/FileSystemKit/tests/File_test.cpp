// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <fstream>
#include <iostream>
#include <span>
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
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeSpanBinary)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename, "wb");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharSpan)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	file.open(tempFilename, "w");
	auto size = file.write(input_data);

	ASSERT_EQ(size, std::size(input_data));

	file.close();

	auto actual_data = readTempFile();

	ASSERT_EQ("abcdef", actual_data);
}

TEST_F(FileTest, writeCharSpanBinary)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

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
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readSpanBinary)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	writeTempFile(input_data);

	file.open(tempFilename, "rb");

	auto output_data = std::array<uint8_t, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readCharSpan)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

	writeTempFile(input_data);

	file.open(tempFilename, "r");

	auto output_data = std::array<char, 6> {};

	auto size = file.read(output_data);

	ASSERT_EQ(std::size(output_data), size);
	ASSERT_EQ(input_data, output_data);
}

TEST_F(FileTest, readCharSpanBinary)
{
	auto input_data = std::to_array<char>({'a', 'b', 'c', 'd', 'e', 'f'});

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
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename);
	auto expected_size = file.write(input_data);

	auto actual_size = file.size();

	ASSERT_EQ(expected_size, actual_size);
}

TEST_F(FileTest, writeThenRead)
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

TEST_F(FileTest, seek)
{
	auto input_data	   = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	auto output_data   = std::array<uint8_t, 6> {};
	auto expected_data = std::array<uint8_t, 6> {0x64, 0x65, 0x66, 0x00, 0x00, 0x00};

	file.open(tempFilename, "w+");

	auto _ = file.write(input_data);
	file.seek(3);
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(3, bytes_read);
	ASSERT_EQ(expected_data, output_data);
}

TEST_F(FileTest, tellNoFile)
{
	auto pos = file.tell();
	ASSERT_EQ(0, pos);
}

TEST_F(FileTest, tellEmptyFile)
{
	file.open(tempFilename);
	auto pos = file.tell();
	ASSERT_EQ(0, pos);
}

TEST_F(FileTest, tellFile)
{
	auto input_data = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"

	file.open(tempFilename);
	auto _ = file.write(input_data);
	file.seek(3);

	auto actual_pos = file.tell();

	ASSERT_EQ(3, actual_pos);
}

TEST_F(FileTest, getFilename)
{
	std::string expected_value = tempFilename;

	auto new_file = FileSystemKit::File {tempFilename};
	auto output_value=new_file.get_filename();

	ASSERT_EQ(expected_value, output_value);
}

TEST_F(FileTest, newLine)
{
	auto input_data_first_line = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"
	auto input_data_second_line = std::to_array<uint8_t>({0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C});	  // "ghijkl"
	auto output_data   = std::array<uint8_t, 14> {};
	auto expected_data = std::array<uint8_t, 14> {0x61, 0x62, 0x63, 0x64, 0x65, 0x66,0x0A, 0x0D,0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C};

	file.open(tempFilename, "w+");
	auto bytes_first_line = file.write(input_data_first_line);
	auto bytes_new_line =file.new_line();
	auto bytes_second_line = file.write(input_data_second_line);

	auto bytes_written=bytes_first_line+bytes_new_line+bytes_second_line;

	file.rewind();
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(bytes_written, bytes_read);
	ASSERT_EQ(expected_data, output_data);

}

TEST_F(FileTest, carriageReturn)
{
	auto input_data_first_line = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	  // "abcdef"
	auto input_data_second_line = std::to_array<uint8_t>({0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C});	  // "ghijkl"
	auto output_data   = std::array<uint8_t, 14> {};
	auto expected_data = std::array<uint8_t, 14> {0x61, 0x62, 0x63, 0x64, 0x65, 0x66,0x0D,0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C};

	file.open(tempFilename, "w+");
	auto bytes_first_line = file.write(input_data_first_line);
	auto bytes_new_line =file.carriage_return();
	auto bytes_second_line = file.write(input_data_second_line);

	auto bytes_written=bytes_first_line+bytes_new_line+bytes_second_line;

	file.rewind();
	auto bytes_read = file.read(output_data);

	ASSERT_EQ(bytes_written, bytes_read);
	ASSERT_EQ(expected_data, output_data);

}
