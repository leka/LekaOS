// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_PLATFORM_FILE_H_
#define _LEKA_OS_INTERFACE_PLATFORM_FILE_H_

#include "../../cxxsupport/lstd_span"

namespace leka::interface {

struct File {
	virtual ~File() = default;

	virtual auto open(const char *path, const char *mode) -> bool = 0;

	virtual void close() = 0;

	virtual auto read(lstd::span<uint8_t> buffer) -> size_t = 0;
	virtual auto write(lstd::span<uint8_t> data) -> size_t	= 0;

	virtual auto read(uint8_t *buffer, uint32_t size) -> size_t = 0;
	virtual auto write(uint8_t *data, uint32_t size) -> size_t	= 0;

	virtual void seek(size_t pos, int origin = SEEK_SET) = 0;

	virtual void rewind() = 0;

	virtual auto size() -> size_t = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_PLATFORM_FILE_H_
