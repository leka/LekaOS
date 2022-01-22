// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_SPI_H_
#define _LEKA_OS_INTERFACE_DRIVER_SPI_H_

#include <cstdint>
#include <span>

namespace leka::interface {

class SPI
{
  public:
	virtual ~SPI() = default;

	virtual auto write(std::span<uint8_t> data) -> size_t = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_SPI_H_
