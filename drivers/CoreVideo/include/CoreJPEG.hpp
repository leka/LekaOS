// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <array>
#include <cstdint>
#include <memory>

#include "CoreJPEGMode.h"
#include "LKCoreFatFsBase.h"
#include "LKCoreSTM32HalBase.h"
#include "external/st_jpeg_utils.h"
#include "interface/JPEG.hpp"

namespace leka {

struct JPEGConfig : JPEG_ConfTypeDef {
	bool initialized = false;
	auto getWidthOffset() const -> uint32_t;
};

class CoreJPEG : public interface::JPEGBase
{
  public:
	CoreJPEG(LKCoreSTM32HalBase &hal, interface::JPEGMode &mode);

	void initialize() final;

	void registerCallbacks() final;

	auto decodeImage(interface::File &file) -> uint32_t final;

	auto getConfig() -> JPEGConfig;

	static auto findFrameOffset(interface::File &file, uint32_t offset) -> uint32_t;

  private:
	LKCoreSTM32HalBase &_hal;
	interface::JPEGMode &_mode;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
