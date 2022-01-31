// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_SPI_H_
#define _LEKA_OS_DRIVER_CORE_SPI_H_

#include "drivers/SPI.h"

#include "interface/drivers/SPI.h"

namespace leka {

class CoreSPI : public interface::SPI
{
  public:
	explicit CoreSPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC) : _spi(mosi, miso, sclk, ssel) {};

	auto write(std::span<const uint8_t> data) -> size_t final;

  private:
	mbed::SPI _spi;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_SPI_H_
