// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"

#include "corevideo_config.h"

namespace leka {

LKCoreJPEG::LKCoreJPEG(LKCoreSTM32HalBase &hal, std::unique_ptr<LKCoreJPEGMode> mode)
	: _hal(hal), _mode(std::move(mode))
{
	_hjpeg.Instance = JPEG;
}

void LKCoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);
}

auto LKCoreJPEG::getConfig() -> JPEG_ConfTypeDef &
{
	return _config;
}

auto LKCoreJPEG::getHandle() -> JPEG_HandleTypeDef &
{
	return _hjpeg;
}

void LKCoreJPEG::registerCallbacks()
{
	static auto* self = this;
	// TODO : register _mode callbacks
}

auto LKCoreJPEG::getWidthOffset(void) -> uint32_t
{
	uint32_t width_offset = 0;

	switch (_config.ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
			if ((_config.ImageWidth % 16) != 0) {
				width_offset = 16 - (_config.ImageWidth % 16);
			}
			break;
		case JPEG_422_SUBSAMPLING:
			if ((_config.ImageWidth % 16) != 0) {
				width_offset = 16 - (_config.ImageWidth % 16);
			}
			break;
		case JPEG_444_SUBSAMPLING:
			if ((_config.ImageWidth % 8) != 0) {
				width_offset = (_config.ImageWidth % 8);
			}
			break;
		default:
			width_offset = 0;
			break;
	}

	return width_offset;
}

auto LKCoreJPEG::decodeImage(LKCoreFatFsBase& file) -> std::uint32_t
{
	return _mode->decodeImage(&_hjpeg, file.getPointer());
}

}	// namespace leka
