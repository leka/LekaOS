// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"

#include "corevideo_config.h"
#include "st_decode_polling.h"
#include "st_jpeg_utils.h"

// Implementation mandatory for HAL
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
	__HAL_RCC_JPEG_CLK_ENABLE();
}

namespace leka {

LKCoreJPEG::LKCoreJPEG(LKCoreDMA2D &dma2d) : _dma2d(dma2d)
{
	_hjpeg.Instance = JPEG;
}

void LKCoreJPEG::initialize()
{
	JPEG_InitColorTables();

	HAL_JPEG_Init(&_hjpeg);
}

void LKCoreJPEG::decodePolling(uint32_t destination_address)
{
	JPEG_DecodePolling(&_hjpeg, _file, destination_address);
}

uint32_t LKCoreJPEG::getWidthOffset()
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

void LKCoreJPEG::display(FIL *jpeg_file)
{
	_file = jpeg_file;
	decodePolling(jpeg::decoded_buffer_address);

	HAL_JPEG_GetInfo(&_hjpeg, &_config);

	_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());
}

FIL *LKCoreJPEG::getFile()
{
	return _file;
}

}	// namespace leka
