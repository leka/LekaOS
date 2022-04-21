// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <filesystem>
#include <span>

#include "CircularQueue.h"
#include "CoreEventQueue.h"
#include "interface/platform/File.h"

namespace leka {

class FileReception
{
  public:
	explicit FileReception(interface::File &file) : _file(file) { event_queue.dispatch_forever(); }

	void setFilePath(const std::filesystem::path &path);
	void onPacketReceived(std::span<uint8_t> buffer);

  private:
	void writeToFile();

	interface::File &_file;

	std::filesystem::path _path {};
	CoreEventQueue event_queue {};

	CircularQueue<uint8_t, 1024> _circular_queue {};
	std::array<uint8_t, 128> _buffer {};
};

}	// namespace leka
