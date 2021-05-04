// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(CoreBufferedSerialTest, initialization)
{
	auto serial		= mbed::BufferedSerial {RFID_UART_TX, RFID_UART_RX, 57600};
	auto coreserial = CoreBufferedSerial {serial};

	ASSERT_NE(&coreserial, nullptr);
}
