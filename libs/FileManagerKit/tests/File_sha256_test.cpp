// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(FileSHA256Test, getSHA256)
{
	auto file = FileManagerKit::File {"fs/README.md", "r"};

	// ? SHA256 got from $shasum -a 256 fs/README.md
	auto expected_sha256 = std::to_array<uint8_t>({
		0xd5, 0x70, 0x15, 0x4c, 0x5a, 0x2d, 0x46, 0x72, 0x46, 0x01, 0xa1, 0x40, 0x3d, 0xcb, 0x0e, 0xe8,
		0x7a, 0x15, 0x0c, 0x2c, 0xb5, 0x34, 0xa3, 0xec, 0x6e, 0x53, 0x02, 0xff, 0x9f, 0x2e, 0x1a, 0xa1,
	});
	auto actual_sha256	 = file.getSHA256();

	EXPECT_EQ(expected_sha256, actual_sha256);
}

TEST(FileSHA256Test, getSHA256FileNotAccessible)
{
	auto file = FileManagerKit::File {"/tmp/unexisting_directory/XXXXXX", "r"};

	std::array<uint8_t, 32> expected_sha256 {};
	expected_sha256.fill('\0');
	auto actual_sha256 = file.getSHA256();

	EXPECT_EQ(expected_sha256, actual_sha256);
}
