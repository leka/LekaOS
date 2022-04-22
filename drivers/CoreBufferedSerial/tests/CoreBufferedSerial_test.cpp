// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(CoreBufferedSerialTest, initialization)
{
	auto coreserial = CoreBufferedSerial {RFID_UART_TX, RFID_UART_RX};

	ASSERT_NE(&coreserial, nullptr);
}
