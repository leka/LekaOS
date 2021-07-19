// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "LKCoreSTM32HalBase.h"
#include "interface/CoreLTDC.h"

namespace leka {

class CoreLTDC : public interface::CoreLTDC
{
  public:
	explicit CoreLTDC(LKCoreSTM32HalBase &hal);

	void initialize() final;

  private:
	LKCoreSTM32HalBase &_hal;

	LTDC_LayerCfgTypeDef _layerConfig;

	void configurePeriphClock();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
