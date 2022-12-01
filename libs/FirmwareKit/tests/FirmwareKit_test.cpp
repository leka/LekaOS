// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <fstream>
#include <iostream>

#include "FirmwareKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/FlashMemory.h"
#include "os_version.h"

using namespace leka;

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Return;

class FirmwareKitTest : public ::testing::Test
{
  protected:
	FirmwareKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	Version current_version = Version {1, 2, 0};

	std::string current_version_str = OS_VERSION;

	mock::FlashMemory mock_flash {};
	FirmwareKit::Config config = {.bin_path_format = "fs/usr/os/LekaOS-%i.%i.%i.bin"};

	FirmwareKit firmwarekit = FirmwareKit {mock_flash, config};
};

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

TEST_F(FirmwareKitTest, loadUpdate)
{
	auto version_to_load = Version {1, 0, 0};
	{
		InSequence seq;

		EXPECT_CALL(mock_flash, erase).Times(1);
		EXPECT_CALL(mock_flash, write).Times(AtLeast(1));
	}

	auto did_load_firmware = firmwarekit.loadUpdate(version_to_load);

	ASSERT_TRUE(did_load_firmware);
}

TEST_F(FirmwareKitTest, loadUpdateFileNotFound)
{
	auto unexisting_version = Version {0, 0, 0};

	auto did_load_firmware = firmwarekit.loadUpdate(unexisting_version);

	ASSERT_FALSE(did_load_firmware);
}

TEST_F(FirmwareKitTest, isVersionAvailable)
{
	auto existing_version	  = Version {1, 0, 0};
	auto is_version_available = firmwarekit.isVersionAvailable(existing_version);

	EXPECT_TRUE(is_version_available);
}

TEST_F(FirmwareKitTest, isVersionAvailableFileNotFound)
{
	auto unexisting_version = Version {0, 0, 0};

	auto is_version_available = firmwarekit.isVersionAvailable(unexisting_version);

	EXPECT_FALSE(is_version_available);
}

TEST_F(FirmwareKitTest, isVersionAvailableFileTooSmall)
{
	auto _config	  = FirmwareKit::Config {.bin_path_format = "/tmp/LekaOS-%i.%i.%i.bin"};
	auto _firmwarekit = FirmwareKit {mock_flash, _config};

	auto dummy_version				   = Version {99, 99, 9999};
	std::string bin_dummy_version_path = "/tmp/LekaOS-99.99.9999.bin";

	auto k_minimal_expected_file_size = 300'000;
	std::ofstream update_stream {bin_dummy_version_path.c_str(), std::ios::binary};
	for (auto i = 0; i < k_minimal_expected_file_size - 1; i++) {
		update_stream << static_cast<uint8_t>(i);
	}
	update_stream.close();

	auto is_version_available = _firmwarekit.isVersionAvailable(dummy_version);

	EXPECT_FALSE(is_version_available);

	std::filesystem::remove(bin_dummy_version_path.c_str());
}
