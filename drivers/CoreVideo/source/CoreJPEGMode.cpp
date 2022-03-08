// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEGMode.h"

namespace leka {

//------------------ Default Callbacks ------------------------
void CoreJPEGMode::onMspInitCallback(JPEG_HandleTypeDef *hjpeg) {}

void CoreJPEGMode::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	switch (info->ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
			if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));
			if ((info->ImageHeight % 16) != 0) info->ImageHeight += (16 - (info->ImageHeight % 16));
			break;

		case JPEG_422_SUBSAMPLING:
			if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));
			if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
			break;

		case JPEG_444_SUBSAMPLING:
			if ((info->ImageWidth % 8) != 0) info->ImageWidth += (8 - (info->ImageWidth % 8));
			if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
			break;
	}

	if (JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &_mcu_number) != HAL_OK) {
		while (true)
			;
	}
}

void CoreJPEGMode::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	while (true)
		;
}

void CoreJPEGMode::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	_hw_decode_ended = true;
}

//------------------ DMA Mode ------------------------
void CoreJPEGDMAMode::onMspInitCallback(JPEG_HandleTypeDef *hjpeg)
{
	// enable DMA2 clock
	__HAL_RCC_DMA2_CLK_ENABLE();

	// input DMA parameters
	_hdma_in.Init.Channel			  = DMA_CHANNEL_9;
	_hdma_in.Init.Direction			  = DMA_MEMORY_TO_PERIPH;
	_hdma_in.Init.PeriphInc			  = DMA_PINC_DISABLE;
	_hdma_in.Init.MemInc			  = DMA_MINC_ENABLE;
	_hdma_in.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	_hdma_in.Init.MemDataAlignment	  = DMA_MDATAALIGN_WORD;
	_hdma_in.Init.Mode				  = DMA_NORMAL;
	_hdma_in.Init.Priority			  = DMA_PRIORITY_HIGH;
	_hdma_in.Init.FIFOMode			  = DMA_FIFOMODE_ENABLE;
	_hdma_in.Init.FIFOThreshold		  = DMA_FIFO_THRESHOLD_FULL;
	_hdma_in.Init.MemBurst			  = DMA_MBURST_INC4;
	_hdma_in.Init.PeriphBurst		  = DMA_PBURST_INC4;

	// init and link DMA IN
	_hdma_in.Instance = DMA2_Stream0;
	HAL_DMA_DeInit(&_hdma_in);
	HAL_DMA_Init(&_hdma_in);
	__HAL_LINKDMA(hjpeg, hdmain, _hdma_in);

	// enable DMA IRQ
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	// Output DMA paramters
	_hdma_out.Init.Channel			   = DMA_CHANNEL_9;
	_hdma_out.Init.Direction		   = DMA_PERIPH_TO_MEMORY;
	_hdma_out.Init.PeriphInc		   = DMA_PINC_DISABLE;
	_hdma_out.Init.MemInc			   = DMA_MINC_ENABLE;
	_hdma_out.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	_hdma_out.Init.MemDataAlignment	   = DMA_MDATAALIGN_WORD;
	_hdma_out.Init.Mode				   = DMA_NORMAL;
	_hdma_out.Init.Priority			   = DMA_PRIORITY_VERY_HIGH;
	_hdma_out.Init.FIFOMode			   = DMA_FIFOMODE_ENABLE;
	_hdma_out.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_FULL;
	_hdma_out.Init.MemBurst			   = DMA_MBURST_INC4;
	_hdma_out.Init.PeriphBurst		   = DMA_PBURST_INC4;

	// init and link DMA OUT
	_hdma_out.Instance = DMA2_Stream1;
	HAL_DMA_DeInit(&_hdma_out);
	HAL_DMA_Init(&_hdma_out);
	__HAL_LINKDMA(hjpeg, hdmaout, _hdma_out);

	// enable DMA IRQ
	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

auto CoreJPEGDMAMode::decodeImage(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> uint32_t
{
	static std::array<uint8_t, jpeg::chunk_size_in * jpeg::in_buffers_nb> BIG_CHUNGUS_OF_MEMORY_IN;
	static std::array<uint8_t, jpeg::chunk_size_out * jpeg::out_buffers_nb> BIG_CHUNGUS_OF_MEMORY_OUT;

	previous_image_size = 0;

	_mcu_number		 = 0;
	_mcu_block_index = 0;
	_hw_decode_ended = false;

	_out_read_index	 = 0;
	_out_write_index = 0;
	_out_paused		 = false;

	_in_read_index	= 0;
	_in_write_index = 0;
	_in_paused		= false;

	// initialize memory chungus
	uint32_t i = 0;
	for (auto &buffer: _in_buffers) {
		buffer.state	= Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data		= BIG_CHUNGUS_OF_MEMORY_IN.data() + i * jpeg::chunk_size_in;
		i += 1;
	}
	i = 0;
	for (auto &buffer: _out_buffers) {
		buffer.state	= Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data		= BIG_CHUNGUS_OF_MEMORY_OUT.data() + i * jpeg::chunk_size_out;
		i += 1;
	}

	// read file and fill input buffers
	for (auto &buffer: _in_buffers) {
		buffer.datasize = file.read(buffer.data, jpeg::chunk_size_in);
		// if (f_read(file, buffer.data, jpeg::chunk_size_in, &buffer.datasize) == FR_OK)
		if (buffer.datasize > 0) {
			buffer.state = Buffer::State::Full;
		}
	}

	// start JPEG decoding with DMA method
	HAL_JPEG_Decode_DMA(hjpeg, _in_buffers[0].data, _in_buffers[0].datasize, _out_buffers[0].data,
						jpeg::chunk_size_out);

	// loop until decode process ends
	bool process_ended = false;
	unsigned in_time   = 0;
	unsigned out_time  = 0;
	do {
		unsigned start = HAL_GetTick();
		decoderInputHandler(hjpeg, file);
		in_time += HAL_GetTick() - start;

		start		  = HAL_GetTick();
		process_ended = decoderOutputHandler(hjpeg);
		out_time += HAL_GetTick() - start;
	} while (!process_ended);

	return previous_image_size;
}

void CoreJPEGDMAMode::onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize)
{
	auto &read_buffer	   = _in_buffers[_in_read_index];
	auto &next_read_buffer = _in_buffers[(_in_read_index + 1) % jpeg::in_buffers_nb];

	if (decoded_datasize == read_buffer.datasize) {
		read_buffer.state	 = Buffer::State::Empty;
		read_buffer.datasize = 0;

		_in_read_index = (_in_read_index + 1) % jpeg::in_buffers_nb;

		if (next_read_buffer.state == Buffer::State::Empty) {
			_in_paused = true;
			HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		} else {
			HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data, next_read_buffer.datasize);
		}
	} else {
		HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data + decoded_datasize,
								   next_read_buffer.datasize - decoded_datasize);
	}
	previous_image_size += decoded_datasize;
}

void CoreJPEGDMAMode::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t output_datasize)
{
	auto &write_buffer		= _out_buffers[_out_write_index];
	auto &next_write_buffer = _out_buffers[(_out_write_index + 1) % jpeg::out_buffers_nb];

	write_buffer.state	  = Buffer::State::Full;
	write_buffer.datasize = output_datasize;

	_out_write_index = (_out_write_index + 1) % jpeg::out_buffers_nb;

	if (next_write_buffer.state != Buffer::State::Empty) {
		_out_paused = true;
		HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}
	HAL_JPEG_ConfigOutputBuffer(hjpeg, next_write_buffer.data, jpeg::chunk_size_out);
}

void CoreJPEGDMAMode::decoderInputHandler(JPEG_HandleTypeDef *hjpeg, interface::File &file)
{
	auto &write_buffer = _in_buffers[_in_write_index];
	auto &read_buffer  = _in_buffers[_in_read_index];

	if (write_buffer.state == Buffer::State::Empty) {
		write_buffer.datasize = file.read(write_buffer.data, jpeg::chunk_size_in);
		// if (f_read(file, write_buffer.data, jpeg::chunk_size_in, &write_buffer.datasize) == FR_OK)
		if (write_buffer.datasize > 0)
			write_buffer.state = Buffer::State::Full;
		else
			while (true)
				;

		if (_in_paused && _in_write_index == _in_read_index) {
			_in_paused = false;
			HAL_JPEG_ConfigInputBuffer(hjpeg, read_buffer.data, read_buffer.datasize);
			HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		}

		_in_write_index = (_in_write_index + 1) % jpeg::in_buffers_nb;
	}
}

auto CoreJPEGDMAMode::decoderOutputHandler(JPEG_HandleTypeDef *hjpeg) -> bool
{
	uint32_t converted_data_count;
	auto &read_buffer  = _out_buffers[_out_read_index];
	auto &write_buffer = _out_buffers[_out_write_index];

	if (read_buffer.state == Buffer::State::Full) {
		_mcu_block_index += pConvert_Function(read_buffer.data, (uint8_t *)jpeg::decoded_buffer_address,
											  _mcu_block_index, read_buffer.datasize, &converted_data_count);

		read_buffer.state	 = Buffer::State::Empty;
		read_buffer.datasize = 0;

		_out_read_index = (_out_read_index + 1) % jpeg::out_buffers_nb;
	} else if (_out_paused && write_buffer.state == Buffer::State::Empty && read_buffer.state == Buffer::State::Empty) {
		_out_paused = false;
		HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}

	return _mcu_block_index == _mcu_number;
}

}	// namespace leka
