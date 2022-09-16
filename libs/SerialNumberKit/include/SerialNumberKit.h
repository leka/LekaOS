// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <interface/drivers/MCU.h>
#include <span>

#include "SerialNumberFormater.h"

namespace leka {

class SerialNumberKit
{
  public:
	explicit SerialNumberKit(interface::MCU &mcu) : _formater(mcu)
	{
		// do nothing
	}

	auto getSerialNumber() -> std::span<uint8_t>;
	auto getShortSerialNumber() -> std::span<uint8_t>;

  private:
	static constexpr std::size_t SN_SIZE	   = 31 + 1;   //  SN + `\0` for const char * use
	static constexpr std::size_t SHORT_SN_SIZE = 15 + 1;   //  ShortSN + `\0` for const char * use

	SerialNumberFormater _formater;

	std::array<uint8_t, SN_SIZE> serial_number {};
	std::array<uint8_t, SHORT_SN_SIZE> short_serial_number {};
};

}	// namespace leka
