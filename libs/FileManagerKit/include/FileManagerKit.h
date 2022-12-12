// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>
#include <span>

#include "platform/NonCopyable.h"

#include "filesystem"
#include "interface/platform/File.h"

namespace leka::FileManagerKit {

struct File : public interface::File, public mbed::NonCopyable<File> {
	File(const char *path = nullptr, const char *mode = "r");
	File(const std::filesystem::path &path, const char *mode = "r");

	auto open(const char *path, const char *mode = "r") -> bool final;
	auto open(const std::filesystem::path &path, const char *mode = "r") -> bool final;
	void close() final;

	auto read(std::span<uint8_t> buffer) -> std::size_t final;
	auto write(std::span<uint8_t> data) -> std::size_t final;

	auto read(std::span<char> buffer) -> std::size_t final;
	auto write(std::span<char> data) -> std::size_t final;

	auto read(uint8_t *buffer, uint32_t size) -> std::size_t final;
	auto write(const uint8_t *data, uint32_t size) -> std::size_t final;

	auto read(char *buffer, uint32_t size) -> std::size_t final;
	auto write(const char *data, uint32_t size) -> std::size_t final;

	void seek(size_t pos, int origin = SEEK_SET) final;
	void rewind() final;
	auto size() -> std::size_t final;

	auto tell() -> std::size_t final;

	auto reopen(const char *path, const char *mode = "r") -> bool final;
	auto reopen(const std::filesystem::path &path, const char *mode = "r") -> bool final;

	auto setBuffer(std::span<char> buffer, int mode = _IOFBF) -> bool final;
	auto setBuffer(char *buffer, uint32_t size = BUFSIZ, int mode = _IOFBF) -> bool final;
	auto unsetBuffer() -> bool final;
	auto flush() -> bool final;

	auto error() -> bool final;
	void clearerr() final;

	[[nodiscard]] auto is_open() const -> bool final;

	auto getSHA256() -> std::array<uint8_t, 32> final;

  private:
	std::unique_ptr<FILE, decltype(&fclose)> _file {nullptr, &fclose};
	std::filesystem::path _path;
};

auto create_directory(const std::filesystem::path &path) -> bool;
auto remove(const std::filesystem::path &path) -> bool;
}	// namespace leka::FileManagerKit
