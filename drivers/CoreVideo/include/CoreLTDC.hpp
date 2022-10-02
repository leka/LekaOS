// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/DSI.hpp"
#include "interface/LTDC.hpp"
#include "interface/drivers/STM32Hal.h"

namespace leka {

class CoreLTDC : public interface::LTDCBase
{
  public:
	explicit CoreLTDC(interface::STM32Hal &hal);

	void initialize() final;

	[[nodiscard]] auto getHandle() -> LTDC_HandleTypeDef & final;
	[[nodiscard]] auto getLayerConfig() const -> LTDC_LayerCfgTypeDef;

  private:
	interface::STM32Hal &_hal;

	LTDC_HandleTypeDef _hltdc {};
	LTDC_LayerCfgTypeDef _layerConfig {};

	void configurePeriphClock();
};

}	// namespace leka
