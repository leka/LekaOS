// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <span>

#include "ConfigKit.h"
#include "FileSystemKit.h"

using namespace leka;

auto FileSystemKit::config::get(const char *path, uint8_t pos) -> size_t
{
	FileSystemKit::File _configuration_file {path, "r"};
	if (!_configuration_file.is_open() || pos >= FileSystemKit::config::max_buffer_size) {
		return -1;
	}
	auto input = std::array<uint8_t, FileSystemKit::config::max_buffer_size> {};
	_configuration_file.read(input);
	_configuration_file.close();
	auto conf_data = input.at(pos);
	return conf_data;
}

auto FileSystemKit::config::set(const char *path, uint8_t data, uint8_t pos) -> void
{
	FileSystemKit::File _configuration_file {path, "w"};
	if (!_configuration_file.is_open() || pos >= FileSystemKit::config::max_buffer_size) {
		return;
	}
	auto output	   = std::array<uint8_t, FileSystemKit::config::max_buffer_size> {};
	output.at(pos) = data;
	_configuration_file.write(output);
	_configuration_file.close();
}

auto FileSystemKit::config::get(std::filesystem::path path, uint8_t pos) -> size_t
{
	FileSystemKit::File _configuration_file {path.c_str(), "r"};
	if (!_configuration_file.is_open() || pos >= FileSystemKit::config::max_buffer_size) {
		return -1;
	}
	auto input = std::array<uint8_t, FileSystemKit::config::max_buffer_size> {};
	_configuration_file.read(input);
	_configuration_file.close();
	auto conf_data = input.at(pos);
	return conf_data;
}

auto FileSystemKit::config::set(std::filesystem::path path, uint8_t data, uint8_t pos) -> void
{
	FileSystemKit::File _configuration_file {path.c_str(), "w"};
	if (!_configuration_file.is_open() || pos >= FileSystemKit::config::max_buffer_size) {
		return;
	}
	auto output	   = std::array<uint8_t, FileSystemKit::config::max_buffer_size> {};
	output.at(pos) = data;
	_configuration_file.write(output);
	_configuration_file.close();
}
