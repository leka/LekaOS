// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <functional>
#include <span>

#include "interface/STM32HalBasicTimer.h"
#include "interface/drivers/DAC.h"
#include "interface/drivers/STM32Hal.h"

namespace leka {

class CoreDAC : public interface::DACBase
{
  public:
	CoreDAC(interface::STM32Hal &hal, interface::STM32HalBasicTimer &hal_timer);

	[[nodiscard]] auto getHandle() -> DAC_HandleTypeDef & final;

	void initialize() final;
	void terminate() final;

	void registerDataToPlay(std::span<uint16_t> data);
	void registerDMACallbacks(std::function<void()> const &on_half_transfer,
							  std::function<void()> const &on_complete_transfer);

	void start() final;
	void stop() final;

  private:
	void _registerMspCallbacks();
	void _initializeDMA();

	interface::STM32Hal &_hal;
	interface::STM32HalBasicTimer &_hal_timer;

	DAC_HandleTypeDef _hdac {};
	DMA_HandleTypeDef _hdma {};

	std::span<uint16_t> _data;
	std::function<void()> _on_half_transfer {};
	std::function<void()> _on_complete_transfer {};
};

}	// namespace leka
