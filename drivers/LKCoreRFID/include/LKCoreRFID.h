// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include <array>

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

struct RFIDTag {
	std::array<uint8_t, 8> UID {0};
	std::array<uint8_t, 2> crc_UID {0};
	std::array<uint8_t, 4> SAK {0};
	std::array<uint8_t, 16> data {0};
};

class LKCoreRFID
{
  public:
	explicit LKCoreRFID(interface::BufferedSerial &interface);

  private:
	interface::BufferedSerial &_interface;
	RFIDTag _rfid_tag {0, 0, 0, 0};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
