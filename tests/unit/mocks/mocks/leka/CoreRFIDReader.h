// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/RFIDReader.h"

namespace leka::mock {

class CoreRFIDReader : public interface::RFIDReader
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, registerTagAvailableCallback, (tagAvailableCallback), (override));
	MOCK_METHOD(void, onDataAvailable, (), (override));
	MOCK_METHOD(bool, setBaudrate, (uint8_t), (override));
	MOCK_METHOD(bool, setCommunicationProtocol, (rfid::Protocol), (override));
	MOCK_METHOD(void, sendCommandToTag, (std::span<uint8_t>), (override));
	MOCK_METHOD(bool, receiveDataFromTag, (std::span<uint8_t> data), (override));
	MOCK_METHOD(void, setModeTagDetection, (), (override));
	MOCK_METHOD(bool, isTagDetected, (), (override));
};

}	// namespace leka::mock
