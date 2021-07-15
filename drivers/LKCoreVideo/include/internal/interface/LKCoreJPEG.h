// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_BASE_H_
#define _LEKA_OS_DRIVER_JPEG_BASE_H_

#include "LKCoreFatFsBase.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka::interface {

class LKCoreJPEG
{
  public:
	~LKCoreJPEG() = default;

	virtual void initialize() = 0;

	virtual auto getHandle() -> JPEG_HandleTypeDef & = 0;
	virtual auto getConfig() -> JPEG_ConfTypeDef	 = 0;

	virtual void registerCallbacks() = 0;

	virtual auto decodeImage(LKCoreFatFsBase &) -> uint32_t = 0;
	// virtual auto getWidthOffset() -> uint32_t			= 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_BASE_H_
