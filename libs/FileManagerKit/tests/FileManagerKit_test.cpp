// Leka - LekaOS
// Copyright 2022 APF France handicap
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

class FileSystemTest : public ::testing::Test
{
  protected:
	void SetUp() override { spy_remove_all_directories(); }
	// void TearDown() override {}

	void spy_mkdir(const std::filesystem::path &path) { std::filesystem::create_directories(path); }

	void spy_rmdir(const std::filesystem::path &path) { std::filesystem::remove_all(path); }

	void spy_create_all_directories()
	{
		std::filesystem::create_directories(path_A);
		std::filesystem::create_directories(path_B);
		std::filesystem::create_directories(path_A1);
		std::filesystem::create_directories(path_B1);
	}

	void spy_remove_all_directories()
	{
		std::filesystem::remove_all(path_A);
		std::filesystem::remove_all(path_B);
		std::filesystem::remove_all(path_A1);
		std::filesystem::remove_all(path_B1);
	}

	const std::filesystem::path path_A	= std::filesystem::temp_directory_path() / "A";
	const std::filesystem::path path_B	= std::filesystem::temp_directory_path() / "B";
	const std::filesystem::path path_A1 = std::filesystem::temp_directory_path() / "A/A1";
	const std::filesystem::path path_B1 = std::filesystem::temp_directory_path() / "B/B1";
};

TEST_F(FileSystemTest, createDirectory)
{
	auto created = FileManagerKit::create_directory(path_A);
	ASSERT_TRUE(created);
}

TEST_F(FileSystemTest, createAlreadyExistingDirectory)
{
	spy_mkdir(path_A);
	auto created = FileManagerKit::create_directory(path_A);
	ASSERT_FALSE(created);
}

TEST_F(FileSystemTest, createSubDirectory)
{
	spy_mkdir(path_A);
	auto created = FileManagerKit::create_directory(path_A1);
	ASSERT_TRUE(created);
}

TEST_F(FileSystemTest, removeDirectory)
{
	spy_rmdir(path_B);
	spy_mkdir(path_B);
	auto removed = FileManagerKit::remove(path_B);
	ASSERT_TRUE(removed);
}

TEST_F(FileSystemTest, removeNotExistingDirectory)
{
	spy_rmdir(path_B);
	auto removed = FileManagerKit::remove(path_B);
	ASSERT_FALSE(removed);
}
