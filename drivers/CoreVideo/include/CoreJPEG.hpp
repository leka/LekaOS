// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "CoreJPEGMode.h"
#include "external/st_jpeg_utils.h"
#include "interface/JPEG.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka {

struct JPEGConfig : JPEG_ConfTypeDef {
	bool initialized = false;
	auto getWidthOffset() const -> uint32_t;
};

class CoreJPEG : public interface::JPEGBase
{
  public:
	CoreJPEG(interface::STM32Hal &hal, interface::JPEGMode &mode);

	void initialize() final;

	auto getHandle() -> JPEG_HandleTypeDef & final;

	void registerCallbacks() final;

	auto decodeImage(interface::File &file) -> uint32_t final;

	auto getConfig() -> JPEGConfig;

	static auto findFrameOffset(interface::File &file, uint32_t offset) -> uint32_t;

  private:
	interface::STM32Hal &_hal;

	interface::JPEGMode &_mode;

	JPEG_HandleTypeDef _hjpeg;
};

}	// namespace leka
