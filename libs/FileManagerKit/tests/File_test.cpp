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

	std::ignore = file.write(input_data);
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

	file.open(tempFilename, "w");
	file.write(input_data);
	file.seek(3);

	auto actual_pos = file.tell();

	ASSERT_EQ(3, actual_pos);
}

TEST_F(FileTest, reopenNoFile)
{
	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTest, openThenReopenFile)
{
	file.open(tempFilename, "r");

	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_TRUE(reopen);
}

TEST_F(FileTest, reopenNoFileWithFileSystemPath)
{
	auto reopen = file.reopen(tempFilename_filesystem_path, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTest, openThenReopenFileWithFileSystemPath)
{
	file.open(tempFilename_filesystem_path, "r");

	auto reopen = file.reopen(tempFilename_filesystem_path, "w");

	ASSERT_TRUE(reopen);
}

TEST_F(FileTest, closeThenReopenFile)
{
	file.open(tempFilename, "r");

	file.close();

	auto reopen = file.reopen(tempFilename, "w");

	ASSERT_FALSE(reopen);
}

TEST_F(FileTest, reopen)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "r");
	file.reopen(tempFilename, "w");
	file.write(input_data);
	file.close();

	auto output_data_w = readTempFile();

	ASSERT_EQ("abcdef", output_data_w);
}

TEST_F(FileTest, setBufferSpanNoFile)
{
	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_FALSE(buffering);
}

TEST_F(FileTest, setBufferSpan)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");

	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_TRUE(buffering);

	file.write(input_data);

	for (int i = 0; i < input_data.size(); i++) {
		ASSERT_EQ(input_data[i], buffer[i]);
	}

	auto output_data = readTempFile();

	ASSERT_EQ("", output_data);
}

TEST_F(FileTest, setBufferCharNoFile)
{
	char buffer[BUFSIZ];

	auto buffering = file.setBuffer(buffer, static_cast<uint32_t>(BUFSIZ));

	ASSERT_FALSE(buffering);
}

TEST_F(FileTest, setBufferChar)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");

	char buffer[BUFSIZ];

	auto buffering = file.setBuffer(buffer, static_cast<uint32_t>(BUFSIZ));

	ASSERT_TRUE(buffering);

	file.write(input_data);

	for (int i = 0; i < input_data.size(); i++) {
		ASSERT_EQ(input_data[i], buffer[i]);
	}

	auto output_data = readTempFile();

	ASSERT_EQ("", output_data);
}

TEST_F(FileTest, unsetBufferNoFile)
{
	auto noBuffering = file.unsetBuffer();

	ASSERT_FALSE(noBuffering);
}

TEST_F(FileTest, unsetBufferFile)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");

	auto noBuffering = file.unsetBuffer();

	ASSERT_TRUE(noBuffering);

	file.write(input_data);

	auto output_data = readTempFile();

	ASSERT_EQ("abcdef", output_data);
}

TEST_F(FileTest, setBufferThenUnsetBuffer)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");

	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_TRUE(buffering);

	file.write(input_data);

	for (int i = 0; i < input_data.size(); i++) {
		ASSERT_EQ(input_data[i], buffer[i]);
	}

	auto output_data_buffer_set = readTempFile();

	ASSERT_EQ("", output_data_buffer_set);

	std::fill(std::begin(buffer), std::end(buffer), 0);

	auto noBuffering = file.unsetBuffer();

	ASSERT_TRUE(noBuffering);

	file.rewind();

	file.write(input_data);

	for (int i = 0; i < input_data.size(); i++) {
		ASSERT_EQ(0, buffer[i]);
	}

	auto output_data_buffer_no_set = readTempFile();

	ASSERT_EQ("abcdef", output_data_buffer_no_set);
}

TEST_F(FileTest, flushNoFile)
{
	auto flush = file.flush();

	ASSERT_FALSE(flush);
}

TEST_F(FileTest, flush)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w+");

	file.write(input_data);

	auto flush = file.flush();

	auto output_data = readTempFile();

	ASSERT_EQ("abcdef", output_data);
}

TEST_F(FileTest, setBufferThenFlush)
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

TEST_F(FileTest, setBufferNoFlushThenFlush)
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
