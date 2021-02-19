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

HAL_StatusTypeDef LKCoreJPEG::decodeImageWithPolling(void)
{
	/* Read from JPG file and fill the input buffer */
	if (f_read(_file, JPEG_InBuffer.DataBuffer, CHUNK_SIZE_IN, (UINT *)(&JPEG_InBuffer.DataBufferSize)) != FR_OK) {
		return HAL_ERROR;
	}

	/* Update the file Offset*/
	Inputfile_Offset = JPEG_InBuffer.DataBufferSize;

	/* Start JPEG decoding with polling (Blocking) method */
	HAL_JPEG_Decode(&_hjpeg, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize, MCU_Data_OutBuffer, CHUNK_SIZE_OUT,
					HAL_MAX_DELAY);

	return HAL_OK;
}

uint32_t LKCoreJPEG::getWidthOffset(void)
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

	// TODO: handle errors
	decodeImageWithPolling();

	HAL_JPEG_GetInfo(&_hjpeg, &_config);

	_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());
}

FIL *LKCoreJPEG::getFile()
{
	return _file;
}

void LKCoreJPEG::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// nothing to do
}

void LKCoreJPEG::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	if (info->ChromaSubsampling == JPEG_420_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));

		if ((info->ImageHeight % 16) != 0) info->ImageHeight += (16 - (info->ImageHeight % 16));
	}

	if (info->ChromaSubsampling == JPEG_422_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));

		if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
	}

	if (info->ChromaSubsampling == JPEG_444_SUBSAMPLING) {
		if ((info->ImageWidth % 8) != 0) info->ImageWidth += (8 - (info->ImageWidth % 8));

		if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
	}

	if (JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &MCU_TotalNb) != HAL_OK) {
		// OnError_Handler();
	}
}

void LKCoreJPEG::onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	if (size != JPEG_InBuffer.DataBufferSize) {
		Inputfile_Offset = Inputfile_Offset - JPEG_InBuffer.DataBufferSize + size;
		f_lseek(LKCoreJPEG::getFile(), Inputfile_Offset);
	}

	if (f_read(LKCoreJPEG::getFile(), JPEG_InBuffer.DataBuffer, CHUNK_SIZE_IN,
			   (UINT *)(&JPEG_InBuffer.DataBufferSize)) == FR_OK) {
		Inputfile_Offset += JPEG_InBuffer.DataBufferSize;
		HAL_JPEG_ConfigInputBuffer(hjpeg, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize);
	} else {
		// OnError_Handler();
	}
}
void LKCoreJPEG::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size)
{
	uint32_t ConvertedDataCount;

	MCU_BlockIndex +=
		pConvert_Function(pDataOut, (uint8_t *)jpeg::decoded_buffer_address, MCU_BlockIndex, size, &ConvertedDataCount);

	HAL_JPEG_ConfigOutputBuffer(hjpeg, MCU_Data_OutBuffer, CHUNK_SIZE_OUT);
}

void LKCoreJPEG::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	// nothing to do
	// TODO: implement flag
}

}	// namespace leka
