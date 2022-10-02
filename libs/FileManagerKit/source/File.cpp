// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <span>

#include "FileManagerKit.h"

using namespace leka::FileManagerKit;

File::File(const char *path, const char *mode)
{
	if (path == nullptr || mode == nullptr) {
		return;
	}

	open(path, mode);
}

File::File(const std::filesystem::path &path, const char *mode)
{
	if (path.empty() || mode == nullptr) {
		return;
	}

	open(path.c_str(), mode);
}

auto File::open(const char *path, const char *mode) -> bool
{
	_file.reset(std::fopen(path, mode));
	return is_open();
}

auto File::open(const std::filesystem::path &path, const char *mode) -> bool
{
	_file.reset(std::fopen(path.c_str(), mode));
	return is_open();
}

void File::close()
{
	if (_file == nullptr) {
		return;
	}

	_file.reset(nullptr);
}

auto File::read(std::span<uint8_t> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(uint8_t), buffer.size(), _file.get());
}

auto File::write(std::span<uint8_t> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(uint8_t), data.size(), _file.get());
}

auto File::read(std::span<char> buffer) -> size_t
{
	return std::fread(buffer.data(), sizeof(char), buffer.size(), _file.get());
}

auto File::write(std::span<char> data) -> size_t
{
	return std::fwrite(data.data(), sizeof(char), data.size(), _file.get());
}

auto File::read(uint8_t *buffer, uint32_t size) -> size_t
{
	return std::fread(buffer, sizeof(uint8_t), size, _file.get());
}

auto File::write(const uint8_t *data, uint32_t size) -> size_t
{
	return std::fwrite(data, sizeof(uint8_t), size, _file.get());
}

auto File::read(char *buffer, uint32_t size) -> size_t
{
	return std::fread(buffer, sizeof(char), size, _file.get());
}

auto File::write(const char *data, uint32_t size) -> size_t
{
	return std::fwrite(data, sizeof(char), size, _file.get());
}

void File::seek(size_t pos, int origin)
{
	std::fseek(_file.get(), static_cast<long>(pos), origin);
}

void File::rewind()
{
	seek(0);
}

auto File::size() -> size_t
{
	if (_file == nullptr) {
		return 0;
	}

	std::fseek(_file.get(), 0, SEEK_END);
	auto size = std::ftell(_file.get());
	std::fseek(_file.get(), 0, SEEK_SET);

	return size;
}

auto File::is_open() const -> bool
{
	return _file != nullptr;
}

auto File::tell() -> size_t
{
	if (_file == nullptr) {
		return 0;
	}
	return std::ftell(_file.get());
}

auto File::reopen(const char *path, const char *mode) -> bool
{
	if (_file == nullptr) {
		return false;
	}
	std::freopen(path, mode, _file.get());
	return is_open();
}

auto File::reopen(const std::filesystem::path &path, const char *mode) -> bool
{
	if (_file == nullptr) {
		return false;
	}
	std::freopen(path.c_str(), mode, _file.get());
	return is_open();
}

auto File::setBuffer(std::span<char> buffer, int mode) -> bool
{
	if (_file == nullptr) {
		return false;
	}
	return (0 == std::setvbuf(_file.get(), buffer.data(), mode, buffer.size()));
}

auto File::setBuffer(char *buffer, uint32_t size, int mode) -> bool
{
	return setBuffer(std::span<char>({buffer, size}), mode);
}

auto File::unsetBuffer() -> bool
{
	if (_file == nullptr) {
		return false;
	}
	return (0 == std::setvbuf(_file.get(), nullptr, _IONBF, 0));
}

auto File::flush() -> bool
{
	if (_file == nullptr) {
		return false;
	}
	return (0 == std::fflush(_file.get()));
}

auto File::error() -> bool
{
	if (_file == nullptr) {
		return false;
	}
	return static_cast<bool>(std::ferror(_file.get()));
}

void File::clearerr()
{
	if (_file == nullptr) {
		return;
	}

	std::clearerr(_file.get());
}
