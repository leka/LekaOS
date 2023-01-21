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

	void spy_readConfigValue(std::span<uint8_t> input_data)
	{
		if (FileManagerKit::File file {config_path.c_str()}; file.is_open()) {
			file.read(input_data);
		}
	}

	auto spy_readConfigValue() -> uint8_t
	{
		auto data = std::array<uint8_t, 1> {};
		if (FileManagerKit::File file {config_path.c_str()}; file.is_open()) {
			file.read(data);
		}
		return data[0];
	}

	void spy_writeConfigValue(std::span<uint8_t> data)
	{
		if (FileManagerKit::File file {config_path.c_str(), "r+"}; file.is_open()) {
			file.write(data);
		}
	}

	void spy_writeConfigValue(uint8_t input_data)
	{
		if (FileManagerKit::File file {config_path.c_str(), "r+"}; file.is_open()) {
			auto data = std::array<uint8_t, 1> {input_data};
			file.write(data);
		}
	}

	void spy_touchConfigFile()
	{
		FileManagerKit::File file {config_path.c_str(), "w"};
		file.close();
	}
	void spy_removeConfigFile() { std::remove(config_path.c_str()); }

	ConfigKit configkit {};
	const std::filesystem::path config_path = "/tmp/test_config.conf";
};

TEST_F(ConfigTest, initializationConfigFullPath)
{
	auto custom_full_path = std::filesystem::path {"/tmp/test_config.conf"};
	auto config			  = Config<5> {custom_full_path};
	auto expected_path	  = std::filesystem::path {"/tmp/test_config.conf"};

	EXPECT_NE(nullptr, &config);

	EXPECT_EQ(expected_path, config.path());
}

TEST_F(ConfigTest, initializationWithDefaultParentPathConfig)
{
	auto custom_filename = std::filesystem::path {"test_config.conf"};
	auto config			 = Config<5> {custom_filename};
	auto expected_path	 = std::filesystem::path {"/fs/etc/test_config.conf"};

	EXPECT_NE(nullptr, &config);
	EXPECT_EQ(expected_path, config.path());
}

TEST_F(ConfigTest, initializationConfigKit)
{
	EXPECT_NE(nullptr, &configkit);
}

TEST_F(ConfigTest, readNotOpenFile)
{
	auto config_buffer = std::array<uint8_t, 5> {"Leka"};
	auto config		   = Config {config_path, config_buffer};

	spy_removeConfigFile();

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(config.default_value(), actual_data_config);
}

TEST_F(ConfigTest, readEmptyFile)
{
	auto config				  = Config<5> {config_path};
	auto expected_data_config = std::array<uint8_t, 5> {};

	spy_removeConfigFile();
	spy_touchConfigFile();

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, read)
{
	auto config				  = Config<5> {config_path};
	auto expected_data_config = std::array<uint8_t, 5> {"Leka"};

	spy_writeConfigValue(expected_data_config);

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, writeCreatingFile)
{
	auto config = Config<5> {config_path};

	auto expected_data_config = std::array<uint8_t, 5> {"Leka"};
	auto actual_data_config	  = std::array<uint8_t, 5> {};

	spy_removeConfigFile();

	auto write = configkit.write(config, expected_data_config);
	EXPECT_TRUE(write);

	spy_readConfigValue(actual_data_config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, writeNotOpenFile)
{
	auto config_buffer			 = std::array<uint8_t, 5> {"Leka"};
	auto unreachable_config_path = std::filesystem::path {"/tmp/unexisting_directory/test_config.conf"};
	auto config					 = Config {unreachable_config_path, config_buffer};

	spy_removeConfigFile();

	auto write = configkit.write(config, {});
	EXPECT_FALSE(write);
}

TEST_F(ConfigTest, write)
{
	auto config = Config<5> {config_path};

	auto expected_data_config = std::array<uint8_t, 5> {"Leka"};
	auto actual_data_config	  = std::array<uint8_t, 5> {};

	spy_touchConfigFile();
	auto write = configkit.write(config, expected_data_config);
	EXPECT_TRUE(write);

	spy_readConfigValue(actual_data_config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, readNotOpenFileOneByteConfig)
{
	auto expected_data_config = uint8_t {0x2A};
	auto config				  = Config {config_path, {expected_data_config}};

	spy_removeConfigFile();

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, readEmptyFileOneByteConfig)
{
	auto expected_data_config = uint8_t {};
	auto config				  = Config {config_path};

	spy_removeConfigFile();
	spy_touchConfigFile();

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, readOneByteConfig)
{
	auto expected_data_config = uint8_t {0x2A};
	auto config				  = Config {config_path};

	spy_writeConfigValue(expected_data_config);

	auto actual_data_config = configkit.read(config);
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, writeCreatingFileOneByteConfig)
{
	auto config = Config {config_path};

	auto expected_data_config = uint8_t {0x2A};

	spy_removeConfigFile();

	auto write = configkit.write(config, {expected_data_config});
	EXPECT_TRUE(write);

	auto actual_data_config = spy_readConfigValue();
	EXPECT_EQ(actual_data_config, expected_data_config);
}

TEST_F(ConfigTest, writeNotOpenFileOneByteConfig)
{
	auto unreachable_config_path = std::filesystem::path {"/tmp/unexisting_directory/test_config.conf"};
	auto config					 = Config {unreachable_config_path, {0x2A}};

	spy_removeConfigFile();

	auto write = configkit.write(config, {});
	EXPECT_FALSE(write);
}

TEST_F(ConfigTest, writeOneByteConfig)
{
	auto config = Config {config_path};

	auto expected_data_config = uint8_t {0x2A};

	spy_touchConfigFile();
	auto write = configkit.write(config, {expected_data_config});
	EXPECT_TRUE(write);

	auto actual_data_config = spy_readConfigValue();
	EXPECT_EQ(actual_data_config, expected_data_config);
}
