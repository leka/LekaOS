// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileSystemTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	void TearDown() override { spy_remove_all_directories(); }

	void spy_create_directory(const std::filesystem::path &path) { std::filesystem::create_directories(path); }
	void spy_remove_directory(const std::filesystem::path &path) { std::filesystem::remove_all(path); }

	void spy_remove_all_directories()
	{
		spy_remove_directory(path_sub_directory);
		spy_remove_directory(path_directory);
	}

	const std::filesystem::path path_directory	   = std::filesystem::temp_directory_path() / "ABC";
	const std::filesystem::path path_sub_directory = std::filesystem::temp_directory_path() / "ABC/DEF";
};

TEST_F(FileSystemTest, createDirectory)
{
	spy_remove_directory(path_directory);

	auto is_created = FileManagerKit::create_directory(path_directory);

	EXPECT_TRUE(is_created);
}

TEST_F(FileSystemTest, createAlreadyExistingDirectory)
{
	spy_create_directory(path_directory);

	auto is_created = FileManagerKit::create_directory(path_directory);

	EXPECT_FALSE(is_created);
}

TEST_F(FileSystemTest, createSubDirectoryInExistingDirectory)
{
	spy_create_directory(path_directory);

	auto is_created = FileManagerKit::create_directory(path_sub_directory);

	EXPECT_TRUE(is_created);
}

TEST_F(FileSystemTest, removeExistingDirectory)
{
	spy_create_directory(path_directory);

	auto is_removed = FileManagerKit::remove(path_directory);

	EXPECT_TRUE(is_removed);
}

TEST_F(FileSystemTest, removeNotExistingDirectory)
{
	spy_remove_directory(path_directory);

	auto is_removed = FileManagerKit::remove(path_directory);

	EXPECT_FALSE(is_removed);
}
