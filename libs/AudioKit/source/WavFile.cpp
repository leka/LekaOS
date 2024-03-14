// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavFile.h"
#include <cstring>

#include "FileManagerKit.h"

using namespace leka;

auto WavFile::open(std::filesystem::path filename) -> bool
{
	auto path = directory / filename;
	path += extension;

	if (FileManagerKit::file_is_missing(path)) {
		return false;
	}

	auto is_open = _file.open(path, "r");

	readHeader();

	_is_eof = false;

	return is_open;
}

void WavFile::readHeader()
{
	std::array<int16_t, sizeof(_header)> buffer {};

	_file.read(buffer);

	std::memcpy(&_header, &buffer, sizeof(_header));
}

auto WavFile::getHeader() const -> const WavHeader &
{
	return _header;
}

auto WavFile::read(std::span<int16_t> buffer) -> bool
{
	auto bytes_read = _file.read(buffer);

	_is_eof			= bytes_read != std::size(buffer);
	auto is_not_eof = !_is_eof;
	return is_not_eof;
}

auto WavFile::isEndOfFile() const -> bool
{
	return _is_eof;
}
