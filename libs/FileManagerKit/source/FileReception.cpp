// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileReception.h"

using namespace leka;

void FileReception::setFilePath(const std::filesystem::path &path)
{
	_path = path;
}

void FileReception::clearFile()
{
	if (_file.open(_path)) {
		_file.clear();
		_file.close();
	}
}

void FileReception::onPacketReceived(std::span<const uint8_t> buffer)
{
	_circular_queue.push(buffer.data(), std::size(buffer));

	event_queue.call([this] { writeToFile(); });
}

void FileReception::writeToFile()
{
	if (_file.open(_path, "a")) {
		_buffer.fill('\0');
		auto data_read = _circular_queue.pop(_buffer.data(), std::size(_buffer));
		_file.write(_buffer.data(), data_read);
		_file.close();
	}
}
