// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "FileSystemKit.h"
#include "stm32f7xx_hal.h"

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
	_file.reset(fopen(path, mode));
	_size_changed = true;
	return is_open();
}

void FileSystemKit::File::close()
{
	if (_file == nullptr) {
		return;
	}

	_file.reset(nullptr);
}

auto FileSystemKit::File::read(lstd::span<uint8_t> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(uint8_t), buffer.size(), _file.get());
}

auto FileSystemKit::File::write(lstd::span<uint8_t> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(uint8_t), data.size(), _file.get());
}

auto FileSystemKit::File::read(lstd::span<char> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(char), buffer.size(), _file.get());
}

auto FileSystemKit::File::write(lstd::span<char> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(char), data.size(), _file.get());
}

auto FileSystemKit::File::read(uint8_t *buffer, uint32_t size) -> size_t
{
	return std::fread(buffer, sizeof(uint8_t), size, _file.get());
}

auto FileSystemKit::File::write(const uint8_t *data, uint32_t size) -> size_t
{
	_size_changed = true;
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

	if (!_size_changed) {
		return _size;
	}

	_size_changed = false;

	auto pos = std::ftell(_file.get());
	std::fseek(_file.get(), 0, SEEK_END);
	_size = std::ftell(_file.get());
	std::fseek(_file.get(), pos, SEEK_SET);

	return _size;
}

auto FileSystemKit::File::is_open() const -> bool
{
	return _file != nullptr;
}
