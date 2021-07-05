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
	registerCallbacks();

	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);

	// need to be called again because JPEG_Init resets the callbacks
	registerCallbacks();

	// enable JPEG IRQ request
	HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
	HAL_NVIC_EnableIRQ(JPEG_IRQn);
}

auto LKCoreJPEG::getConfig(bool update) -> JPEG_ConfTypeDef &
{
	if (update) _hal.HAL_JPEG_GetInfo(&_hjpeg, &_config);
	
	return _config;
}

auto LKCoreJPEG::getHandle() -> JPEG_HandleTypeDef &
{
	return _hjpeg;
}

void LKCoreJPEG::registerCallbacks()
{
	static auto *self = this;
	HAL_JPEG_RegisterInfoReadyCallback(&self->getHandle(), [](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
		self->_mode->onInfoReadyCallback(hjpeg, info);
	});

	HAL_JPEG_RegisterGetDataCallback(&self->getHandle(), [](JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) {
		self->_mode->onGetDataCallback(hjpeg, decoded_datasize);
	});

	HAL_JPEG_RegisterDataReadyCallback(&self->getHandle(),
									   [](JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) {
										   self->_mode->onDataReadyCallback(hjpeg, output_data, datasize);
									   });

	HAL_JPEG_RegisterCallback(&self->getHandle(), HAL_JPEG_DECODE_CPLT_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg) { self->_mode->onDecodeCompleteCallback(hjpeg); });

	HAL_JPEG_RegisterCallback(&self->getHandle(), HAL_JPEG_ERROR_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg) { self->_mode->onErrorCallback(hjpeg); });

	HAL_JPEG_RegisterCallback(&self->getHandle(), HAL_JPEG_MSPINIT_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg) { self->_mode->onMspInitCallback(hjpeg); });
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

auto LKCoreJPEG::decodeImage(LKCoreFatFsBase &file) -> std::uint32_t
{
	return _mode->decodeImage(&_hjpeg, file.getPointer());
}

}	// namespace leka
