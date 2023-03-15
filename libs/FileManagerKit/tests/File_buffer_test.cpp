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

class FileTestBuffer : public ::testing::Test
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

TEST_F(FileTestBuffer, setBufferSpanNoFile)
{
	auto buffer = std::array<char, BUFSIZ> {};

	auto buffering = file.setBuffer(buffer);

	ASSERT_FALSE(buffering);
}

TEST_F(FileTestBuffer, setBufferSpan)
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

TEST_F(FileTestBuffer, setBufferCharNoFile)
{
	char buffer[BUFSIZ];

	auto buffering = file.setBuffer(buffer, static_cast<uint32_t>(BUFSIZ));

	ASSERT_FALSE(buffering);
}

TEST_F(FileTestBuffer, setBufferChar)
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

TEST_F(FileTestBuffer, unsetBufferNoFile)
{
	auto noBuffering = file.unsetBuffer();

	ASSERT_FALSE(noBuffering);
}

TEST_F(FileTestBuffer, unsetBufferFile)
{
	auto input_data = std::to_array<char>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});   // "abcdef"

	file.open(tempFilename, "w");

	auto noBuffering = file.unsetBuffer();

	ASSERT_TRUE(noBuffering);

	file.write(input_data);

	auto output_data = readTempFile();

	ASSERT_EQ("abcdef", output_data);
}

TEST_F(FileTestBuffer, setBufferThenUnsetBuffer)
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
