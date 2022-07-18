// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <fstream>
#include <iostream>

#include "FirmwareKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/FlashMemory.h"

using namespace leka;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class FirmwareKitTest : public ::testing::Test
{
  protected:
	FirmwareKitTest() : firmwarekit(mock_flash, "/tmp/update-v%i.%i.%i") {}

	void SetUp() override
	{
		std::ofstream update {"/tmp/update-v1.2.3", std::ios::binary};
		update.write(content.data(), std::size(content));
		update.close();
	}
	void TearDown() override { std::remove("/tmp/update-v1.2.3"); }

	mock::FlashMemory mock_flash;
	FirmwareKit firmwarekit;

	FirmwareVersion default_current_version = FirmwareVersion {1, 0, 0};

	std::array<char, 6> content = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};	  // "abcdef"
};

MATCHER_P2(compareArray, expected_array, useless_var, "")
{
	bool same_content = true;

	for (int i = 0; i < expected_array.size(); i++) {
		same_content &= expected_array[i] == arg[i];
	}

	return same_content;
}

TEST_F(FirmwareKitTest, instantiation)
{
	ASSERT_NE(&firmwarekit, nullptr);
}

TEST_F(FirmwareKitTest, getCurrentVersion)
{
	auto actual_version = firmwarekit.getCurrentVersion();

	EXPECT_EQ(actual_version.major, default_current_version.major);
	EXPECT_EQ(actual_version.minor, default_current_version.minor);
	EXPECT_EQ(actual_version.revision, default_current_version.revision);
}

TEST_F(FirmwareKitTest, loadUpdate)
{
	auto version = FirmwareVersion {.major = 1, .minor = 2, .revision = 3};

	{
		InSequence seq;

		EXPECT_CALL(mock_flash, erase).Times(1);
		EXPECT_CALL(mock_flash, write(_, compareArray(content, _), std::size(content))).Times(1);
	}

	auto did_load_firmware = firmwarekit.loadUpdate(version);

	ASSERT_TRUE(did_load_firmware);
}

TEST_F(FirmwareKitTest, loadUpdateFileNotFound)
{
	auto version = FirmwareVersion {.major = 1, .minor = 2, .revision = 3};

	std::remove("/tmp/update-v1.2.3");

	auto did_load_firmware = firmwarekit.loadUpdate(version);

	ASSERT_FALSE(did_load_firmware);
}
