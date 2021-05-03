// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {
RFIDTag LKCoreRFID::RFIDMessageIntoStruct(uint8_t *tag_value)
{
	RFIDTag decompose_value;

	uint8_t length = tag_value[1];
	if (!isDataLengthOk(length)) {
		return {0, 0, 0};
	}

	decompose_value.command = tag_value[0];
	decompose_value.length	= length;

	for (int i = 0; i < decompose_value.length; ++i) {
		decompose_value.data[i] = tag_value[i + 2];
	}

	return decompose_value;
}

auto LKCoreRFID::isDataLengthOk(uint8_t length) -> bool
{
	if (length > _MAXIMUM_BUFFER_SIZE) {
		return false;
	}
	return true;
}

}	// namespace leka
