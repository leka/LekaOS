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
	MOCK_METHOD(bool, isTagDetected, (), (override));
	MOCK_METHOD(void, setTagDetectionMode, (), (override));
	MOCK_METHOD(void, setCommunicationProtocol, (rfid::Protocol), (override));
	MOCK_METHOD(void, sendToTag, (std::span<const uint8_t>), (override));
	MOCK_METHOD(bool, didTagCommunicationSucceed, (size_t), (override));
	MOCK_METHOD((std::array<uint8_t, 18>), getDataFromTag, (), (override));
	MOCK_METHOD(rfid::Tag, getTag, (), (override));
	MOCK_METHOD(void, onTagDataReceived, (), (override));
	MOCK_METHOD(void, registerOnTagDetectedCallback, (const std::function<void()> &), (override));
	MOCK_METHOD(void, registerOnTagReadableCallback, (const std::function<void()> &), (override));
};

}	// namespace leka::mock
