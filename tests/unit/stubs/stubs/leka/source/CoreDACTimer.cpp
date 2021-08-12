// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal, HardWareBasicTimer tim) : _hal(hal), _hardwareTim(tim)
{
	// nothing to do
}

void CoreDACTimer::initialize(uint32_t frequency)
{
	// nothing to do
}

void CoreDACTimer::terminate()
{
	// nothing to do
}

void CoreDACTimer::start()
{
	// nothing to do
}

void CoreDACTimer::stop()
{
	// nothing to do
}

auto CoreDACTimer::getHandle() const -> const TIM_HandleTypeDef &
{
	return this->_htim;
}

auto CoreDACTimer::getHardWareBasicTimer() const -> const HardWareBasicTimer &
{
	auto *tim = new HardWareBasicTimer(HardWareBasicTimer::BasicTimer6);
	return *tim;
}

auto CoreDACTimer::_calculatePeriod(uint32_t frequency) -> uint32_t
{
	return 108000000 / 44100;
}

void CoreDACTimer::_registerMspCallbacks()
{
	// nothing to do
}

void CoreDACTimer::_msp_onInitializationCb()
{
	// nothing to do
}

void CoreDACTimer::_msp_onTerminationCb()
{
	// nothing to do
}

}	// namespace leka
