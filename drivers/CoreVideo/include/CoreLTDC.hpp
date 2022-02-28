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
	CoreLTDC(interface::STM32Hal &hal, interface::DSIBase &dsi);

	void initialize() final;

	[[nodiscard]] auto getHandle() const -> LTDC_HandleTypeDef;
	[[nodiscard]] auto getLayerConfig() const -> LTDC_LayerCfgTypeDef;

  private:
	interface::STM32Hal &_hal;
	interface::DSIBase &_dsi;

	LTDC_HandleTypeDef _hltdc {};
	LTDC_LayerCfgTypeDef _layerConfig {};

	void configurePeriphClock();
};

}	// namespace leka
