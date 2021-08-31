// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_H_
#define _LEKA_OS_DRIVER_DSI_H_

#include "LKCoreSTM32HalBase.h"
#include "interface/CoreDSI.h"

namespace leka {

class CoreDSI : public interface::CoreDSI
{
  public:
	explicit CoreDSI(LKCoreSTM32HalBase &hal);

	void initialize() final;
	void start() final;
	void reset() final;

	DSI_HandleTypeDef getHandle() const;
	DSI_VidCfgTypeDef getConfig() final;

	void write(const uint8_t *data, const uint32_t size) final;

  private:
	LKCoreSTM32HalBase &_hal;
	DSI_HandleTypeDef _hdsi;
	DSI_VidCfgTypeDef _config;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DSI_H_
