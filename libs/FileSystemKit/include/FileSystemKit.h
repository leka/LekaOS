// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FILE_SYSTEM_KIT_H_
#define _LEKA_OS_LIB_FILE_SYSTEM_KIT_H_

#include <memory>

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

		auto read(lstd::span<uint8_t> buffer) -> size_t final;
		auto write(lstd::span<uint8_t> data) -> size_t final;

		auto read(lstd::span<char> buffer) -> size_t final;
		auto write(lstd::span<char> data) -> size_t final;

		auto read(uint8_t *buffer, uint32_t size) -> size_t final;
		auto write(const uint8_t *data, uint32_t size) -> size_t final;

		auto read(char *buffer, uint32_t size) -> size_t final;
		auto write(const char *data, uint32_t size) -> size_t final;

		void seek(size_t pos, int origin = SEEK_SET) final;
		void rewind() final;
		auto size() -> size_t final;

		[[nodiscard]] auto is_open() const -> bool final;

	  private:
		std::unique_ptr<FILE, decltype(&fclose)> _file {nullptr, fclose};
	};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FILE_SYSTEM_KIT_H_
