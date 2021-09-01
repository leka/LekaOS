// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "LKCoreSTM32HalBase.h"
#include "interface/CoreDSI.hpp"
#include "interface/CoreLTDC.hpp"

namespace leka {

class CoreLTDC : public interface::CoreLTDC
{
  public:
	CoreLTDC(LKCoreSTM32HalBase &hal, interface::CoreDSI &dsi);

	void initialize() final;

	[[nodiscard]] auto getHandle() const -> LTDC_HandleTypeDef;
	[[nodiscard]] auto getLayerConfig() const -> LTDC_LayerCfgTypeDef;

  private:
	LKCoreSTM32HalBase &_hal;
	interface::CoreDSI &_dsi;

	LTDC_HandleTypeDef _hltdc {};
	LTDC_LayerCfgTypeDef _layerConfig {};

	void configurePeriphClock();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
