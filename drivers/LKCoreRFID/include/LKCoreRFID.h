// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

class LKCoreRFID
{
  public:
	LKCoreRFID(interface::BufferedSerial &interface);

	auto writeProtocol() -> void;
	auto checkProtocol() -> bool;

  private:
	interface::BufferedSerial &_interface;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
