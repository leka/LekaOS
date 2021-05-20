// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {

LKCoreRFID::LKCoreRFID(interface::BufferedSerial &interface) : _interface(interface) {}

auto LKCoreRFID::setRFIDTag(RFIDTag const &expected_values) -> void
{
	_rfid_tag = expected_values;
}

auto LKCoreRFID::getRFIDTag() const -> RFIDTag
{
	return _rfid_tag;
}

}	// namespace leka
