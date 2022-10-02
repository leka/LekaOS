// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialNumberKit.h"

using namespace leka;

auto SerialNumberKit::getSerialNumber() -> std::span<uint8_t>
{
	auto prefix_size	   = _formater.setPrefix(serial_number);
	auto date_of_test_size = _formater.setDateOfTest(serial_number, prefix_size);
	auto number_of_digits  = static_cast<uint8_t>(SN_SIZE - (prefix_size + date_of_test_size + sizeof('\0')));

	_formater.setMCUID(serial_number, prefix_size + date_of_test_size, number_of_digits);

	return serial_number;
}

auto SerialNumberKit::getShortSerialNumber() -> std::span<uint8_t>
{
	auto prefix_size	   = _formater.setPrefix(short_serial_number);
	auto date_of_test_size = _formater.setDateOfTest(short_serial_number, prefix_size);
	auto number_of_digits  = static_cast<uint8_t>(SHORT_SN_SIZE - (prefix_size + date_of_test_size + sizeof('\0')));

	_formater.setMCUID(short_serial_number, prefix_size + date_of_test_size, number_of_digits);

	return short_serial_number;
}
