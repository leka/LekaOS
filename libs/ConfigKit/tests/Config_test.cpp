// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <fstream>
#include <iostream>
#include <span>
#include <string>

#include "ConfigKit.h"
#include "FileManagerKit.h"
#include "LogKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/File.h"

using namespace leka;

class ConfigTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	auto spy_readConfigValue() -> uint8_t
	{
		auto input_data = std::array<uint8_t, 1> {};
		FileManagerKit::File file {config_path.c_str()};
		file.read(input_data);
		file.close();
		return input_data.front();
	}

	void spy_writeConfigValue(uint8_t data)
	{
		auto output_data = std::array<uint8_t, 1> {data};
		FileManagerKit::File file {config_path.c_str(), "r+"};
		file.write(output_data);
		file.close();
	}

	void spy_touchConfigFile()
	{
		FileManagerKit::File file {config_path.c_str(), "w"};
		file.close();
	}
	void spy_removeConfigFile() { std::remove(config_path.c_str()); }

	const std::filesystem::path config_path = "/tmp/test_config.conf";
};

TEST_F(ConfigTest, initializationConfigFullPath)
{
	const std::filesystem::path custom_full_path = "/tmp/test_config.conf";
	Config config {custom_full_path};
	ASSERT_NE(nullptr, &config);
	const std::filesystem::path expected_path = "/tmp/test_config.conf";
	ASSERT_EQ(expected_path, config.path());
}

TEST_F(ConfigTest, initializationWithDefaultParentPathConfig)
{
	const std::filesystem::path custom_filename = "test_config.conf";
	Config config {custom_filename};
	ASSERT_NE(nullptr, &config);
	const std::filesystem::path expected_path = "/fs/etc/test_config.conf";
	ASSERT_EQ(expected_path, config.path());
}

TEST_F(ConfigTest, initializationConfigKit)
{
	auto configkit = ConfigKit();
	ASSERT_NE(nullptr, &configkit);
}

TEST_F(ConfigTest, readNotOpenFile)
{
	Config config {config_path};
	auto configkit = ConfigKit();
	spy_removeConfigFile();
	auto data = configkit.read(config);
	ASSERT_EQ(config.default_value(), data);
}

TEST_F(ConfigTest, readEmptyFile)
{
	Config config {config_path};
	auto configkit = ConfigKit();
	spy_removeConfigFile();
	spy_touchConfigFile();
	auto data = configkit.read(config);
	ASSERT_EQ(0, data);
}

TEST_F(ConfigTest, read)
{
	Config config {config_path};
	auto configkit = ConfigKit();
	spy_writeConfigValue(5);
	auto data = configkit.read(config);
	ASSERT_EQ(5, data);
}

TEST_F(ConfigTest, writeCreatingFile)
{
	Config config {config_path};
	auto configkit = ConfigKit();

	spy_removeConfigFile();

	auto write = configkit.write(config, 5);
	ASSERT_TRUE(write);

	auto data = spy_readConfigValue();
	ASSERT_EQ(5, data);
}

TEST_F(ConfigTest, writeNotOpenFile)
{
	auto unreachable_config_path = std::filesystem::path {"/tmp/unnexisting_directory/test_config.conf"};
	Config config {unreachable_config_path};
	auto configkit = ConfigKit();
	spy_removeConfigFile();
	auto write = configkit.write(config, 5);
	ASSERT_FALSE(write);
}

TEST_F(ConfigTest, write)
{
	Config config {config_path};
	auto configkit = ConfigKit();
	spy_touchConfigFile();
	auto write = configkit.write(config, 5);
	ASSERT_TRUE(write);
	auto data = spy_readConfigValue();
	ASSERT_EQ(5, data);
}
