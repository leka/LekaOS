// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LKComponentHandler.h"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka::interface {

class JPEGBase : public LKComponentHandler<JPEG_HandleTypeDef>
{
  public:
	~JPEGBase() = default;

	virtual void initialize() = 0;

	virtual auto getConfig() -> JPEG_ConfTypeDef = 0;

	virtual void registerCallbacks() = 0;

	virtual auto decodeImage(interface::File &) -> uint32_t = 0;
};

}	// namespace leka::interface
