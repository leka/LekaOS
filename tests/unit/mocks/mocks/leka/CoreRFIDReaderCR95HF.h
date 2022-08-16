// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/RFIDReader.h"

namespace leka::mock {

class CoreRFIDReaderCR95HF : public interface::RFIDReader
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, setModeTagDetection, (), (override));
	MOCK_METHOD(void, setCommunicationProtocol, (rfid::Protocol), (override));
	MOCK_METHOD(void, sendRequestToTag, (std::span<const uint8_t>), (override));
	MOCK_METHOD(bool, didTagCommunicationSucceed, (size_t), (override));
	MOCK_METHOD(rfid::Tag &, getTag, (), (override));
	MOCK_METHOD(void, onTagReadable, (), (override));
	MOCK_METHOD(void, registerOnTagDetectedCallback, (const std::function<void()> &), (override));
	MOCK_METHOD(void, registerOnTagReadableCallback, (const std::function<void(rfid::Tag &)> &), (override));
};

}	// namespace leka::mock
