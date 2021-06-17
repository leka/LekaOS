// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"
#include "LogKit.h"

#include "corevideo_config.h"
#include "decode_dma.h"

void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
  static DMA_HandleTypeDef   hdmaIn;
  static DMA_HandleTypeDef   hdmaOut;
  
  /* Enable JPEG clock */
  __HAL_RCC_JPEG_CLK_ENABLE();
  
    /* Enable DMA clock */
  __HAL_RCC_DMA2_CLK_ENABLE();

  HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
  HAL_NVIC_EnableIRQ(JPEG_IRQn);
  
  /* Input DMA */    
  /* Set the parameters to be configured */
  hdmaIn.Init.Channel = DMA_CHANNEL_9;
  hdmaIn.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdmaIn.Init.PeriphInc = DMA_PINC_DISABLE;
  hdmaIn.Init.MemInc = DMA_MINC_ENABLE;
  hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaIn.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdmaIn.Init.Mode = DMA_NORMAL;
  hdmaIn.Init.Priority = DMA_PRIORITY_HIGH;
  hdmaIn.Init.FIFOMode = DMA_FIFOMODE_ENABLE;         
  hdmaIn.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdmaIn.Init.MemBurst = DMA_MBURST_INC4;
  hdmaIn.Init.PeriphBurst = DMA_PBURST_INC4;      
  
  hdmaIn.Instance = DMA2_Stream3;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmain, hdmaIn);
  
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);    
  
  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&hdmaIn);  
  /* Initialize the DMA stream */
  HAL_DMA_Init(&hdmaIn);
  
  
  /* Output DMA */
  /* Set the parameters to be configured */ 
  hdmaOut.Init.Channel = DMA_CHANNEL_9;
  hdmaOut.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdmaOut.Init.PeriphInc = DMA_PINC_DISABLE;
  hdmaOut.Init.MemInc = DMA_MINC_ENABLE;
  hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaOut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdmaOut.Init.Mode = DMA_NORMAL;
  hdmaOut.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdmaOut.Init.FIFOMode = DMA_FIFOMODE_ENABLE;         
  hdmaOut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdmaOut.Init.MemBurst = DMA_MBURST_INC4;
  hdmaOut.Init.PeriphBurst = DMA_PBURST_INC4;

  
  hdmaOut.Instance = DMA2_Stream4;
  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&hdmaOut);  
  /* Initialize the DMA stream */
  HAL_DMA_Init(&hdmaOut);

  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmaout, hdmaOut);
  
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);   
    
}

extern uint32_t Previous_FrameSize;

namespace leka {

LKCoreJPEG::LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file)
	: _hal(hal), _dma2d(dma2d), _file(file)
{
	_hjpeg.Instance = JPEG;
}

void LKCoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);
}

JPEG_ConfTypeDef LKCoreJPEG::getConfig(void)
{
	return _config;
}

JPEG_HandleTypeDef LKCoreJPEG::getHandle(void)
{
	return _hjpeg;
}

JPEG_HandleTypeDef *LKCoreJPEG::getHandlePointer(void)
{
	return &_hjpeg;
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

void LKCoreJPEG::displayImage(FIL *file)
{
	registerDmaCallbacks();

	auto start_time = HAL_GetTick();
	_previous_frame_size = 0;

	decodeImageWithDma();	// TODO: handle errors

	_hal.HAL_JPEG_GetInfo(&_hjpeg, &_config);

	_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());

	log_info("Image time : %dms", HAL_GetTick() - start_time);
}

uint32_t findFrameOffset(uint32_t offset, LKCoreFatFsBase &file)
{
	uint8_t pattern_search_buffer[jpeg::PATTERN_SEARCH_BUFFERSIZE];

	uint32_t index     = offset;
	uint32_t read_size = 0;

	do {
		if (file.getSize() <= (index + 1)) {
			return 0;
		}
		file.seek(index);
		file.read(pattern_search_buffer, jpeg::PATTERN_SEARCH_BUFFERSIZE, &read_size);

		if (read_size != 0) {
			for (uint32_t i = 0; i < (read_size - 1); i++) {
				if ((pattern_search_buffer[i] == jpeg::JPEG_SOI_MARKER_BYTE1) &&
					(pattern_search_buffer[i + 1] == jpeg::JPEG_SOI_MARKER_BYTE0)) {
					return index + i;
				}
			}
			index += (read_size - 1);
		}
	} while (read_size != 0);

	return 0;
}


void LKCoreJPEG::playVideo()
{
	registerDmaCallbacks();

	bool is_first_frame	  = true;
	uint32_t frame_index  = 0;
	uint32_t frame_offset = 0;

	do {
		auto start_time = HAL_GetTick();
		frame_offset = findFrameOffset(frame_offset + Previous_FrameSize, _file);
		if (frame_offset != 0) {

			_file.seek(frame_offset);
			Previous_FrameSize = 0;
			decodeImageWithDma();

			frame_index++;

			if (is_first_frame) {
				is_first_frame = false;
				HAL_JPEG_GetInfo(&_hjpeg, &_config);
			}
			_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());

			log_info("framenb : %d, Frame time %dms", frame_index,HAL_GetTick() - start_time);
		}
	} while (frame_offset != 0);
}


HAL_StatusTypeDef LKCoreJPEG::decodeImageWithPolling(void)
{
	// WARNING: DO NOT REMOVE
	_mcu_block_index = 0;

	// TODO: rely on LKFileSystemKit to handle open/read/close
	uint32_t read_size;
	if (_file.read(_jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) != FR_OK) {
		return HAL_ERROR;
	}
	_input_file_offset = read_size;

	_hal.HAL_JPEG_Decode(&_hjpeg,
						_jpeg_input_buffer.data(), read_size,
						_jpeg_output_buffer.data(), _jpeg_output_buffer.size(),
						HAL_MAX_DELAY);
	return HAL_OK;
}

HAL_StatusTypeDef LKCoreJPEG::decodeImageWithDma(void)
{
	decode_dma::JPEG_Decode_DMA(&_hjpeg, _file.getPointer(), jpeg::decoded_buffer_address);
	int jpeg_decode_end = 0;

	do {
		decode_dma::JPEG_InputHandler(&_hjpeg);
		jpeg_decode_end = decode_dma::JPEG_OutputHandler(&_hjpeg);
	} while(jpeg_decode_end == 0);
	
	return HAL_OK;
}

void LKCoreJPEG::registerPollingCallbacks(void) {
	static auto* self = this;
	HAL_JPEG_RegisterInfoReadyCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
			self->polling_onInfoReadyCallback(hjpeg, info);
		}
	);

	HAL_JPEG_RegisterGetDataCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, uint32_t size) {
			self->polling_onDataAvailableCallback(hjpeg, size);
		}
	);

	HAL_JPEG_RegisterDataReadyCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size) {
			self->polling_onDataReadyCallback(hjpeg, pDataOut, size);
		}
	);

	HAL_JPEG_RegisterCallback(
		&_hjpeg, HAL_JPEG_DECODE_CPLT_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->polling_onDecodeCompleteCallback(hjpeg);
		}
	);

	HAL_JPEG_RegisterCallback(
		&_hjpeg, HAL_JPEG_ERROR_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->polling_onErrorCallback(hjpeg);
		}
	);
}

void LKCoreJPEG::registerDmaCallbacks(void) {
	static auto* self = this;
	HAL_JPEG_RegisterInfoReadyCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
			self->dma_onInfoReadyCallback(hjpeg, info);
		}
	);

	HAL_JPEG_RegisterGetDataCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, uint32_t size) {
			self->dma_onDataAvailableCallback(hjpeg, size);
		}
	);

	HAL_JPEG_RegisterDataReadyCallback(
		&_hjpeg,
		[](JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size) {
			self->dma_onDataReadyCallback(hjpeg, pDataOut, size);
		}
	);

	HAL_JPEG_RegisterCallback(
		&_hjpeg, HAL_JPEG_DECODE_CPLT_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->dma_onDecodeCompleteCallback(hjpeg);
		}
	);

	HAL_JPEG_RegisterCallback(
		&_hjpeg, HAL_JPEG_ERROR_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->dma_onErrorCallback(hjpeg);
		}
	);
}

void LKCoreJPEG::polling_onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
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

	if (JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &_mcu_number) != HAL_OK) {
		// TODO: handle errors
	}
}

void LKCoreJPEG::polling_onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	// TODO: rely on LKFileSystemKit to handle open/read/close
	if (size != _jpeg_input_buffer.size()) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size() + size;
		_file.seek(_input_file_offset);
	}

	uint32_t read_size;
	if (_file.read(_jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) == FR_OK) {
		_input_file_offset += read_size;
		_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_input_buffer.data(), read_size);
	} else {
		// TODO: handle error
		log_error("FILE READ ERROR");
	}
	_previous_frame_size += size;
}
void LKCoreJPEG::polling_onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size)
{
	_mcu_block_index +=
		pConvert_Function(pDataOut, (uint8_t *)jpeg::decoded_buffer_address, _mcu_block_index, size, nullptr);

	_hal.HAL_JPEG_ConfigOutputBuffer(hjpeg, _jpeg_output_buffer.data(), _jpeg_output_buffer.size());
}

void LKCoreJPEG::polling_onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO: implement flag
}

void LKCoreJPEG::polling_onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO: handle errors
}

void LKCoreJPEG::dma_onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
	decode_dma::InfoReadyCallback(hjpeg, info);
}
void LKCoreJPEG::dma_onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) {
	decode_dma::GetDataCallback(hjpeg, size);
}
void LKCoreJPEG::dma_onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) {
	decode_dma::DataReadyCallback(hjpeg, output_buffer, size);
}
void LKCoreJPEG::dma_onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) {
	decode_dma::DecodeCpltCallback(hjpeg);
}
void LKCoreJPEG::dma_onErrorCallback(JPEG_HandleTypeDef *hjpeg) {
	decode_dma::ErrorCallback(hjpeg);
}

}	// namespace leka
