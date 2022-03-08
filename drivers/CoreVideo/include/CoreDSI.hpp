// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/DSI.hpp"
#include "interface/drivers/STM32Hal.h"

namespace leka {

class CoreDSI : public interface::DSIBase
{
  public:
	explicit CoreDSI(interface::STM32Hal &hal);

	void initialize() final;
	void start() final;
	void reset() final;
	void refresh() final;

	auto getSyncProps() -> SyncProps final;

	void enableLPCmd() final;
	void disableLPCmd() final;

	void enableTearingEffectReporting() final;

	[[nodiscard]] auto getHandle() -> DSI_HandleTypeDef & final;

	void write(const uint8_t *data, uint32_t size) final;

  private:
	interface::STM32Hal &_hal;
	DSI_HandleTypeDef _hdsi;
	DSI_CmdCfgTypeDef _cmdconf;
	DSI_LPCmdTypeDef _lpcmd;

	int _screen_sections = 1;
};

}	// namespace leka
