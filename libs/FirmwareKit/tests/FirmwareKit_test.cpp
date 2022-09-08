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
	FirmwareKitTest() = default;

	void SetUp() override
	{
		std::ofstream osv_stream {os_version_path.c_str(), std::ios::binary};
		osv_stream << os_version_str;
		osv_stream.close();

		std::ofstream update_stream {bin_update_path.c_str(), std::ios::binary};
		for (const auto &val: bin_update_content) {
			update_stream << val;
		}
		update_stream.close();
	}
	void TearDown() override { std::filesystem::remove(bin_update_path_format.c_str()); }

	std::string bin_update_path_format		  = "/tmp/update-v%i.%i.%i";
	std::string bin_update_path				  = "/tmp/update-v2.0.0";
	std::array<uint8_t, 6> bin_update_content = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};	// "abcdef"

	std::string os_version_path = "/tmp/os-version";
	std::string os_version_str	= "1.2.3";

	FirmwareVersion default_version = FirmwareVersion {1, 0, 0};
	FirmwareVersion current_version = FirmwareVersion {1, 2, 3};
	FirmwareVersion update_version	= FirmwareVersion {2, 0, 0};

	mock::FlashMemory mock_flash {};

	FirmwareKit firmwarekit = FirmwareKit {mock_flash, bin_update_path_format.c_str(), os_version_path.c_str()};
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

	EXPECT_EQ(actual_version.major, current_version.major);
	EXPECT_EQ(actual_version.minor, current_version.minor);
	EXPECT_EQ(actual_version.revision, current_version.revision);
}

TEST_F(FirmwareKitTest, getCurrentVersionFileNotFound)
{
	std::filesystem::remove(os_version_path.c_str());

	auto actual_version = firmwarekit.getCurrentVersion();

	EXPECT_EQ(actual_version.major, default_version.major);
	EXPECT_EQ(actual_version.minor, default_version.minor);
	EXPECT_EQ(actual_version.revision, default_version.revision);
}

TEST_F(FirmwareKitTest, loadUpdate)
{
	{
		InSequence seq;

		EXPECT_CALL(mock_flash, erase).Times(1);
		EXPECT_CALL(mock_flash, write(_, compareArray(bin_update_content, _), std::size(bin_update_content))).Times(1);
	}

	auto did_load_firmware = firmwarekit.loadUpdate(update_version);

	ASSERT_TRUE(did_load_firmware);
}

TEST_F(FirmwareKitTest, loadUpdateFileNotFound)
{
	std::filesystem::remove(bin_update_path.c_str());

	auto did_load_firmware = firmwarekit.loadUpdate(update_version);

	ASSERT_FALSE(did_load_firmware);
}
