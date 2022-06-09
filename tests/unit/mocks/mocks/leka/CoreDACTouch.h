// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreBufferedSerial.h"
#include "gmock/gmock.h"

namespace leka {

class CoreDACTouchMock : public interface::DACTouch
{
  public:
	MOCK_METHOD(void, setVoltageReference, (uint8_t), (override));
	MOCK_METHOD(void, setPowerMode, (uint8_t), (override));
	MOCK_METHOD(void, setGain, (uint8_t), (override));

	MOCK_METHOD(void, writeToAllInputRegister, (std::array<uint8_t, 2>), (override));
	MOCK_METHOD(void, writeToSpecificInputRegister, (uint8_t, std::array<uint8_t, 2>), (override));

	MOCK_METHOD(void, writeToMemoryRegisterUntilChannelD, (uint8_t, std::array<uint8_t, 2>), (override));
	MOCK_METHOD(void, writeToSpecificMemoryRegister, (uint8_t, std::array<uint8_t, 2>), (override));

	MOCK_METHOD(void, readMemory, (std::array<uint8_t, 24> &), (override));
};

}	// namespace leka
