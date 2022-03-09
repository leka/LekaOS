// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

namespace leka::interface {

struct File {
	virtual ~File() = default;

	virtual auto open(const char *path, const char *mode) -> bool = 0;

	virtual void close() = 0;

	virtual auto read(std::span<uint8_t> buffer) -> size_t = 0;
	virtual auto write(std::span<uint8_t> data) -> size_t  = 0;

	virtual auto read(std::span<char> buffer) -> size_t = 0;
	virtual auto write(std::span<char> data) -> size_t	= 0;

	virtual auto read(uint8_t *buffer, uint32_t size) -> size_t		 = 0;
	virtual auto write(const uint8_t *data, uint32_t size) -> size_t = 0;

	virtual auto read(char *buffer, uint32_t size) -> size_t	  = 0;
	virtual auto write(const char *data, uint32_t size) -> size_t = 0;

	virtual void seek(size_t pos, int origin) = 0;

	virtual void rewind() = 0;

	virtual auto size() -> size_t = 0;

	virtual auto clear() -> bool = 0;

	[[nodiscard]] virtual auto is_open() const -> bool = 0;
};

}	// namespace leka::interface
