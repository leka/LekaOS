// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#define _DARWIN_C_SOURCE
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <span>

#include "FileSystemKit.h"

using namespace leka;

FileSystemKit::File::File(const char *path, const char *mode)
{
	if (path == nullptr || mode == nullptr) {
		return;
	}

	open(path, mode);
}

auto FileSystemKit::File::open(const char *path, const char *mode) -> bool
{
	_file.reset(std::fopen(path, mode));
	return is_open();
}

void FileSystemKit::File::close()
{
	if (_file == nullptr) {
		return;
	}

	_file.reset(nullptr);
}

auto FileSystemKit::File::read(std::span<uint8_t> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(uint8_t), buffer.size(), _file.get());
}

auto FileSystemKit::File::write(std::span<uint8_t> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(uint8_t), data.size(), _file.get());
}

auto FileSystemKit::File::read(std::span<char> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(char), buffer.size(), _file.get());
}

auto FileSystemKit::File::write(std::span<char> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(char), data.size(), _file.get());
}

auto FileSystemKit::File::read(uint8_t *buffer, uint32_t size) -> size_t
{
	return std::fread(buffer, sizeof(uint8_t), size, _file.get());
}

auto FileSystemKit::File::write(const uint8_t *data, uint32_t size) -> size_t
{
	return std::fwrite(data, sizeof(uint8_t), size, _file.get());
}

auto FileSystemKit::File::read(char *buffer, uint32_t size) -> size_t
{
	return std::fread(buffer, sizeof(char), size, _file.get());
}

auto FileSystemKit::File::write(const char *data, uint32_t size) -> size_t
{
	return std::fwrite(data, sizeof(char), size, _file.get());
}

void FileSystemKit::File::seek(size_t pos, int origin)
{
	std::fseek(_file.get(), static_cast<long>(pos), origin);
}

void FileSystemKit::File::rewind()
{
	seek(0);
}

auto FileSystemKit::File::size() -> size_t
{
	if (_file == nullptr) {
		return 0;
	}

	std::fseek(_file.get(), 0, SEEK_END);
	auto size = std::ftell(_file.get());
	std::fseek(_file.get(), 0, SEEK_SET);

	return size;
}

auto FileSystemKit::File::is_open() const -> bool
{
	return _file != nullptr;
}

auto FileSystemKit::File::no() -> size_t
{
	if (_file == nullptr) {
		return -1;
	}
	return fileno(_file.get());
}

auto FileSystemKit::File::path(uint32_t fd, char *bufferpath, uint32_t size) -> size_t
{
	return fcntl(fd, size, bufferpath);
}

auto FileSystemKit::File::path(uint32_t fd, std::span<char> bufferpath) -> size_t
{
	return fcntl(fd, bufferpath.size(), bufferpath.data());
}
