// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"

#include "lcd_properties.h"
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

void LKCoreJPEG::getJpegInformation()
{
	HAL_JPEG_GetInfo(&_hjpeg, &_config);

	// Image centered
	uint16_t x_corner_top_left = (lcd::dimension.width - _config.ImageWidth) / 2;
	uint16_t y_corner_top_left = (lcd::dimension.height - _config.ImageHeight) / 2;

	_dma2d_params.mode			 = LKCoreDMA2D::Mode::M2M_PFC;
	_dma2d_params.pdata_or_color = jpeg::decoded_buffer_address;
	_dma2d_params.destination_address =
		lcd::frame_buffer_address + (y_corner_top_left * lcd::dimension.width + x_corner_top_left) * 4;
	_dma2d_params.xsize			= _config.ImageWidth;
	_dma2d_params.ysize			= _config.ImageHeight;
	_dma2d_params.output_offset = lcd::dimension.width - _config.ImageWidth;
	_dma2d_params.width_offset	= getWidthOffset();
}

void LKCoreJPEG::display(FIL *jpeg_file)
{
	_file = jpeg_file;
	decodePolling(jpeg::decoded_buffer_address);

	getJpegInformation();

	_dma2d.load(_dma2d_params);
}

FIL *LKCoreJPEG::getFile()
{
	return _file;
}

}	// namespace leka
