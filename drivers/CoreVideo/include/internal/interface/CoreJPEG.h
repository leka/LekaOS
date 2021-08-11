// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_BASE_H_
#define _LEKA_OS_DRIVER_JPEG_BASE_H_

#include "ComponentHandler.h"
#include "LKCoreFatFsBase.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka::interface {

class CoreJPEG : public ComponentHandler<JPEG_HandleTypeDef>
{
  public:
	~CoreJPEG() = default;

	virtual void initialize() = 0;

	virtual auto getConfig() -> JPEG_ConfTypeDef = 0;

	virtual void registerCallbacks() = 0;

	virtual auto decodeImage(LKCoreFatFsBase &) -> uint32_t = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_JPEG_BASE_H_
