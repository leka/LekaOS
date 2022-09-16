// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstring>

#include "FileManagerKit.h"
#include "SerialNumberFormater.h"

using namespace leka;

auto SerialNumberFormater::setPrefix(std::span<uint8_t> partial_serial_number) const -> std::size_t
{
	auto prefix = std::array<uint8_t, 3> {'L', 'K', '-'};
	std::copy(prefix.begin(), prefix.end(), partial_serial_number.begin());

	return std::size(prefix);
}

auto SerialNumberFormater::setDateOfTest(std::span<uint8_t> partial_serial_number, uint32_t offset) const -> std::size_t
{
	auto date_of_test = std::array<uint8_t, 4> {};
	date_of_test.fill('X');

	if (auto file = FileManagerKit::File {_config.date_of_test_path}; file.is_open()) {
		file.read(date_of_test);
		file.close();
	}

	std::copy(date_of_test.begin(), date_of_test.end(), partial_serial_number.begin() + offset);

	return std::size(date_of_test);
}

void SerialNumberFormater::setMCUID(std::span<uint8_t> partial_serial_number, uint32_t offset, uint8_t number_of_digits)
{
	constexpr auto MAX_MCU_ID_DIGITS = 24;
	constexpr auto MAX_MCU_ID_LENGTH = MAX_MCU_ID_DIGITS + sizeof('\0');
	std::array<char, MAX_MCU_ID_LENGTH> mcu_id_array {};

	auto mcu_id = _mcu.getID();
	snprintf(mcu_id_array.begin(), std::size(mcu_id_array), "%08lX%08lX%08lX", mcu_id.front, mcu_id.middle,
			 mcu_id.back);

	if (number_of_digits > MAX_MCU_ID_DIGITS) {
		number_of_digits = MAX_MCU_ID_DIGITS;
	}
	std::memcpy(partial_serial_number.data() + offset, &mcu_id_array, number_of_digits);
}
