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
	struct Config {
		const char *date_of_test_path;
	};

	explicit SerialNumberFormater(interface::MCU &mcu, Config config) : _mcu(mcu), _config(config) {}

	[[nodiscard]] auto setPrefix(std::span<uint8_t> serial_number) const -> std::size_t;
	[[nodiscard]] auto setDateOfTest(std::span<uint8_t> serial_number, uint32_t offset) const -> std::size_t;
	void setMCUID(std::span<uint8_t> serial_number, uint32_t offset, uint8_t number_of_digits);

  private:
	void setDateOfTestFromYYMMDD(std::span<char> content, std::span<uint8_t> date_of_test) const;
	void setDateOfTestFromEpoch(std::span<char> content, std::span<uint8_t> date_of_test) const;

	interface::MCU &_mcu;
	const Config _config;
};

}	// namespace leka
