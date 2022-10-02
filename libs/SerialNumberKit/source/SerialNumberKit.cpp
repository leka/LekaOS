// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialNumberKit.h"
#include <cstdio>
#include <cstring>

#include "CastUtils.h"

using namespace leka;

auto SerialNumberKit::getSerialNumber() -> std::array<uint8_t, SN_SIZE>
{
	auto prefix = utils::cast::from_c_string_to_uint8_t_array("LK-22xx-");
	std::copy(prefix.begin(), prefix.end(), serial_number.begin());

	std::array<char, SN_SIZE - std::size(prefix) + 1> mcu_id_array {};
	auto mcu_id = _mcu.getID();
	snprintf(mcu_id_array.begin(), std::size(mcu_id_array), "%08lX%08lX%08lX", mcu_id.front, mcu_id.middle,
			 mcu_id.back);

	std::memcpy(serial_number.begin() + std::size(prefix) - 1, &mcu_id_array, std::size(mcu_id_array));

	return serial_number;
}
