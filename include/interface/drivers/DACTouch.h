// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>

namespace leka::interface {

class DACTouch
{
  public:
	virtual ~DACTouch()																						= default;
	virtual void writeToAllInputRegister(std::array<uint8_t, 2> value)										= 0;
	virtual void writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value)				= 0;
	virtual void writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value) = 0;
	virtual void writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value)				= 0;

	virtual void writeVoltageReference(uint8_t voltageReference) = 0;
	virtual void writePowerMode(uint8_t powerMode)				 = 0;
	virtual void writeGain(uint8_t gain)						 = 0;

	virtual void readMemory(std::array<uint8_t, 24> &value) = 0;
};

}	// namespace leka::interface
