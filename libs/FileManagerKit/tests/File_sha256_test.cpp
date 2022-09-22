// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "FileManagerKit.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(FileSHA256Test, getSHA256)
{
	auto file = FileManagerKit::File {"fs/usr/os/LekaOS-1.0.0.bin", "r"};

	// ? SHA256 got from $shasum -a 256 fs/usr/os/LekaOS-1.0.0.bin
	auto expected_sha256 = std::to_array<uint8_t>({
		0x02, 0x4d, 0x59, 0x65, 0x4e, 0xca, 0x26, 0xf4, 0xfa, 0x21, 0xb7, 0x29, 0x26, 0x81, 0xf6, 0x9f,
		0xdb, 0xd2, 0x3f, 0x3a, 0x82, 0xb7, 0xbc, 0x8f, 0x1f, 0x66, 0xbf, 0xf9, 0x64, 0x84, 0x2c, 0xa8,
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
