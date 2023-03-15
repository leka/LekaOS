// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <filesystem>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

class FileTestInitialization : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		strcpy(tempFilename, "/tmp/XXXXXX");
		mkstemp(tempFilename);
		tempFilename_filesystem_path = tempFilename;
	}
	// void TearDown() override {}

	char tempFilename[L_tmpnam];   // NOLINT
	std::filesystem::path tempFilename_filesystem_path {};
};

TEST_F(FileTestInitialization, initializationDefault)
{
	auto new_file = FileManagerKit::File {};
	ASSERT_NE(&new_file, nullptr);
	ASSERT_FALSE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithPath)
{
	auto new_file = FileManagerKit::File {tempFilename};
	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithPathAndMode)
{
	auto new_file = FileManagerKit::File {tempFilename, "r"};

	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithFileSystemPath)
{
	auto new_file = FileManagerKit::File {tempFilename_filesystem_path};
	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithFileSystemPathAndMode)
{
	auto new_file = FileManagerKit::File {tempFilename_filesystem_path, "r"};

	ASSERT_NE(&new_file, nullptr);
	ASSERT_TRUE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithFileSystemPathAndNullPtrMode)
{
	auto new_file = FileManagerKit::File {tempFilename_filesystem_path, nullptr};

	ASSERT_NE(&new_file, nullptr);
	ASSERT_FALSE(new_file.is_open());
}

TEST_F(FileTestInitialization, initializationWithEmptyAndNullptr)
{
	{
		auto f = FileManagerKit::File {"tempFilename", "r"};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileManagerKit::File {nullptr, nullptr};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileManagerKit::File {"nullptr", nullptr};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileManagerKit::File {nullptr, "r"};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto f = FileManagerKit::File {nullptr};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto empty_path = std::filesystem::path {};
		auto f			= FileManagerKit::File {empty_path};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto empty_path = std::filesystem::path {};
		auto f			= FileManagerKit::File {empty_path, "r"};
		ASSERT_FALSE(f.is_open());
	}
	{
		auto empty_path = std::filesystem::path {};
		auto f			= FileManagerKit::File {empty_path, nullptr};
		ASSERT_FALSE(f.is_open());
	}
}
