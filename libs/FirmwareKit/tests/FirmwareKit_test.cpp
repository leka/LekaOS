// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"
#include <lstd_array>

#include "gtest/gtest.h"
#include "mocks/leka/FlashMemory.h"
#include "stubs/leka/TmpFile.h"

using namespace leka;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class FirmwareKitTest : public ::testing::Test
{
  protected:
	FirmwareKitTest() : firmwarekit(flash_memory) {}

	void SetUp() override { tmp_file.createTempFile(); }
	// void TearDown() override {}

	TmpFile tmp_file;
	mock::FlashMemory flash_memory;
	FirmwareKit firmwarekit;
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

TEST_F(FirmwareKitTest, loadUpdate)
{
	auto expected_is_loaded			  = true;
	auto version					  = FirmwareVersion {.major = 1, .minor = 2, .revision = 3};
	std::array<uint8_t, 6> input_data = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66};	// "abcdef"

	auto path = tmp_file.getPath();

	firmwarekit.setDefaultPath(path);
	tmp_file.writeTempFile(input_data);

	{
		InSequence seq;

		EXPECT_CALL(flash_memory, erase).Times(1);
		EXPECT_CALL(flash_memory, write(_, compareArray(input_data, _), std::size(input_data))).Times(1);
	}

	auto actual_is_loaded = firmwarekit.loadUpdate(version);

	ASSERT_EQ(actual_is_loaded, expected_is_loaded);
}

TEST_F(FirmwareKitTest, loadUpdateExpectedMaxStep)
{
	auto expected_is_loaded = true;
	auto expected_adress	= 0x0;
	auto expected_max_step	= 0x100;
	auto version			= FirmwareVersion {.major = 1, .minor = 2, .revision = 3};
	std::array<uint8_t, 0x110> input_data;

	auto path = tmp_file.getPath();

	firmwarekit.setDefaultPath(path);
	tmp_file.writeTempFile(input_data);

	{
		InSequence seq;

		EXPECT_CALL(flash_memory, erase).Times(1);

		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(1);
		expected_adress += expected_max_step;
		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(1);
	}

	auto actual_is_loaded = firmwarekit.loadUpdate(version);

	ASSERT_EQ(actual_is_loaded, expected_is_loaded);
}

TEST_F(FirmwareKitTest, loadUpdateEmptyPath)
{
	auto expected_is_loaded = false;
	auto version			= FirmwareVersion {.major = 1, .minor = 2, .revision = 3};

	std::string path = "";

	firmwarekit.setDefaultPath(path);

	auto actual_is_loaded = firmwarekit.loadUpdate(version);

	ASSERT_EQ(actual_is_loaded, expected_is_loaded);
}
