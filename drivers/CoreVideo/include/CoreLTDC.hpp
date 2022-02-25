// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreSTM32HalBase.h"
#include "interface/DSI.hpp"
#include "interface/LTDC.hpp"

namespace leka {

class CoreLTDC : public interface::LTDCBase
{
  public:
	CoreLTDC(CoreSTM32HalBase &hal, interface::DSIBase &dsi);

	void initialize() final;

	[[nodiscard]] auto getHandle() const -> LTDC_HandleTypeDef;
	[[nodiscard]] auto getLayerConfig() const -> LTDC_LayerCfgTypeDef;

  private:
	CoreSTM32HalBase &_hal;
	interface::DSIBase &_dsi;

	LTDC_HandleTypeDef _hltdc {};
	LTDC_LayerCfgTypeDef _layerConfig {};

	void configurePeriphClock();
};

}	// namespace leka
