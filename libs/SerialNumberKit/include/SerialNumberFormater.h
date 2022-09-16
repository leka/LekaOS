// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/MCU.h>
#include <span>

namespace leka {

class SerialNumberFormater
{
  public:
	explicit SerialNumberFormater(interface::MCU &mcu) : _mcu(mcu) {}

	[[nodiscard]] auto setPrefix(std::span<uint8_t> serial_number) const -> std::size_t;
	[[nodiscard]] auto setDateOfTest(std::span<uint8_t> serial_number, uint32_t offset) const -> std::size_t;
	void setMCUID(std::span<uint8_t> serial_number, uint32_t offset, uint8_t number_of_digits);

  private:
	interface::MCU &_mcu;
};

}	// namespace leka
