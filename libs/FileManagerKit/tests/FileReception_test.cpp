// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileReception.h"
#include <fstream>

#include "mocks/leka/EventQueue.h"

using namespace leka;

class FileReceptionTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(temp_file_path, "/tmp/XXXXXX");
		mkstemp(temp_file_path);
	}
	// void TearDown() override {}

	mock::EventQueue event_queue {};

	FileReception file_reception;

	static inline auto packet			= std::to_array<uint8_t>({'a', 'b', 'c'});
	static inline auto packet_in_string = std::string(packet.begin(), packet.end());

	char temp_file_path[L_tmpnam];	 // NOLINT

	auto readTempFile() -> std::string
	{
		std::fstream f {};
		f.open(temp_file_path);

		std::stringstream out {};
		out << f.rdbuf();
		f.close();

		return out.str();
	}

	void writeTempFile(std::span<uint8_t> data)
	{
		auto *file = fopen(temp_file_path, "wb");
		fwrite(data.data(), sizeof(uint8_t), data.size(), file);
		fclose(file);
	}
};

TEST_F(FileReceptionTest, instantiation)
{
	EXPECT_NE(&file_reception, nullptr);
}

TEST_F(FileReceptionTest, setFilePath)
{
	writeTempFile(packet);
	auto read_before_action = readTempFile();
	EXPECT_EQ(read_before_action, packet_in_string);

	file_reception.setFilePath(temp_file_path);

	auto read_after_action = readTempFile();
	EXPECT_EQ(read_after_action, "");
}

TEST_F(FileReceptionTest, setFilePathFileNotAccessible)
{
	file_reception.setFilePath("/tmp/unexisting_directory/XXXXXX");

	// nothing expected
}

TEST_F(FileReceptionTest, onPacketReceived)
{
	file_reception.setFilePath(temp_file_path);

	auto read_before_action = readTempFile();
	EXPECT_EQ(read_before_action, "");

	file_reception.onPacketReceived(packet);

	auto read_after_action = readTempFile();
	EXPECT_EQ(read_after_action, packet_in_string);
}

TEST_F(FileReceptionTest, onPacketReceivedFileNotAccessible)
{
	file_reception.setFilePath("/tmp/unexisting_directory/XXXXXX");

	file_reception.onPacketReceived(packet);

	// nothing expected
}
