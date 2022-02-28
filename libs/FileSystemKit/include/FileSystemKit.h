// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>
#include <span>

#include "platform/NonCopyable.h"

#include "interface/platform/File.h"

namespace leka {

class FileSystemKit
{
  public:
	struct File : public interface::File, public mbed::NonCopyable<File> {
		File(const char *path = nullptr, const char *mode = "r");

		auto open(const char *path, const char *mode = "r") -> bool final;
		void close() final;

		auto read(std::span<uint8_t> buffer) -> size_t final;
		auto write(std::span<uint8_t> data) -> size_t final;

		auto read(std::span<char> buffer) -> size_t final;
		auto write(std::span<char> data) -> size_t final;

		auto read(uint8_t *buffer, uint32_t size) -> size_t final;
		auto write(const uint8_t *data, uint32_t size) -> size_t final;

		auto read(char *buffer, uint32_t size) -> size_t final;
		auto write(const char *data, uint32_t size) -> size_t final;

		void seek(size_t pos, int origin = SEEK_SET) final;
		void rewind() final;
		auto size() -> size_t final;

		auto tell() -> size_t final;

		auto rename(const char * new_name) -> size_t final;

		auto new_line() -> size_t final;

		auto carriage_return() -> size_t final;

		[[nodiscard]] auto is_open() const -> bool final;

	  protected:
		std::unique_ptr<FILE, decltype(&fclose)> _file {nullptr, fclose};
	};
};

}	// namespace leka
