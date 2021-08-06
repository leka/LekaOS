// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_DAC_TOUCH_H_
#define _LEKA_OS_INTERFACE_DRIVER_DAC_TOUCH_H_

namespace leka::interface {

class CoreDACTouch
{
  public:
	virtual ~CoreDACTouch() = default;

	virtual void setVoltageReference(uint8_t voltageReference) = 0;
	virtual void setPowerMode(uint8_t powerMode)			   = 0;
	virtual void setGain(uint8_t gain)						   = 0;

	virtual void writeToAllInputRegister(std::array<uint8_t, 2> value)						 = 0;
	virtual void writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value) = 0;

	virtual void writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value) = 0;
	virtual void writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value)				= 0;

	virtual void readMemory(std::array<uint8_t, 24> &value) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_DAC_TOUCH_H_
