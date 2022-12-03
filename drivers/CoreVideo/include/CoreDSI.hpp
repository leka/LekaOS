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

	[[nodiscard]] auto getHandle() const -> DSI_HandleTypeDef;
	[[nodiscard]] auto getConfig() -> DSI_VidCfgTypeDef final;

	[[deprecated]] void write(const uint8_t *data, uint32_t size) final;
	void write(std::span<const uint8_t> data) final;

  private:
	interface::STM32Hal &_hal;
	DSI_HandleTypeDef _hdsi {};
	DSI_VidCfgTypeDef _config {};
};

}	// namespace leka
