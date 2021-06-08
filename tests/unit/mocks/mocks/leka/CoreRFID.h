// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_RFID_MOCK_H_
#define _LEKA_OS_RFID_MOCK_H_

#include <cstddef>

#include "gmock/gmock.h"
#include "interface/drivers/RFID.h"

namespace leka {

class CoreRFIDMock : public interface::RFID
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, registerTagAvailableCallback, (tagAvailableCallback), (override));
	MOCK_METHOD(void, onTagAvailable, (), (override));
	MOCK_METHOD((std::array<uint8_t, 17>), getIDN, (), (override));
	MOCK_METHOD(bool, setBaudrate, (uint8_t), (override));
	MOCK_METHOD(bool, setCommunicationProtocol, (rfid::Protocol), (override));
	MOCK_METHOD(void, sendCommandToTag, (lstd::span<uint8_t>), (override));
	MOCK_METHOD(size_t, receiveDataFromTag, (lstd::span<uint8_t> *), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_RFID_MOCK_H_
