// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SD_H_
#define _LEKA_OS_LIB_SD_H_

#include <chrono>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

using namespace mbed;
using namespace std::chrono;

class LekaSD
{
  public:
	LekaSD();
	~LekaSD() {};

  private:
};

#endif
