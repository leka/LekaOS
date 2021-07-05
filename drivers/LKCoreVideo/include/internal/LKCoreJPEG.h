// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <cstdint>
#include <memory>

#include "LKCoreFatFsBase.h"
#include "LKCoreJPEGBase.h"
#include "LKCoreJPEGMode.h"
#include "LKCoreSTM32HalBase.h"
#include "st_jpeg_utils.h"

namespace leka {

class LKCoreJPEG : public LKCoreJPEGBase
{
  public:
	LKCoreJPEG(LKCoreSTM32HalBase &hal, std::unique_ptr<LKCoreJPEGMode> mode);

	void initialize() final;

	auto getHandle() -> JPEG_HandleTypeDef & final;
	auto getConfig() -> JPEG_ConfTypeDef & final;

	void registerCallbacks() final;

	auto decodeImage(LKCoreFatFsBase &file) -> uint32_t final;
	auto getWidthOffset() -> uint32_t final;

  private:
	LKCoreSTM32HalBase &_hal;

	std::unique_ptr<LKCoreJPEGMode> _mode;

	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
