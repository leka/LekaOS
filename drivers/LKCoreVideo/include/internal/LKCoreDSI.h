// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_H_
#define _LEKA_OS_DRIVER_DSI_H_

#include "LKCoreDSIBase.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreDSI : public LKCoreDSIBase
{
  public:
	explicit LKCoreDSI(LKCoreSTM32HalBase &hal);

	void initialize() final;
	void start() final;
	void reset() final;
	void refresh() final;

	auto getSyncProps() -> SyncProps final;

	void enableLPCmd() final;
	void disableLPCmd() final;

	void enableTearingEffectReporting() final;

	auto getHandle() -> DSI_HandleTypeDef & final;

	void write(const uint8_t *data, const uint32_t size) final;

  private:
	LKCoreSTM32HalBase &_hal;
	DSI_HandleTypeDef _hdsi;
	DSI_CmdCfgTypeDef _cmdconf;
	DSI_LPCmdTypeDef _lpcmd;

	int _screen_sections = 1;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DSI_H_
