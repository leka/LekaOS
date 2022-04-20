// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEGModeDMA.hpp"

using namespace leka;

void CoreJPEGModeDMA::onMspInitCallback(JPEG_HandleTypeDef *hjpeg)
{
	// enable DMA2 clock
	_hal.HAL_RCC_DMA2_CLK_ENABLE();

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
	_hal.HAL_DMA_DeInit(&_hdma_in);
	_hal.HAL_DMA_Init(&_hdma_in);
	__HAL_LINKDMA(hjpeg, hdmain, _hdma_in);

	// enable DMA IRQ
	_hal.HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x07, 0x0F);
	_hal.HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
	_hal.HAL_DMA_DeInit(&_hdma_out);
	_hal.HAL_DMA_Init(&_hdma_out);
	__HAL_LINKDMA(hjpeg, hdmaout, _hdma_out);

	// enable DMA IRQ
	_hal.HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0x07, 0x0F);
	_hal.HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

void CoreJPEGModeDMA::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	if (info->ChromaSubsampling == JPEG_420_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) {
			info->ImageWidth += (16 - (info->ImageWidth % 16));
		}

		if ((info->ImageHeight % 16) != 0) {
			info->ImageHeight += (16 - (info->ImageHeight % 16));
		}
	}

	if (info->ChromaSubsampling == JPEG_422_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) {
			info->ImageWidth += (16 - (info->ImageWidth % 16));
		}

		if ((info->ImageHeight % 8) != 0) {
			info->ImageHeight += (8 - (info->ImageHeight % 8));
		}
	}

	if (info->ChromaSubsampling == JPEG_444_SUBSAMPLING) {
		if ((info->ImageWidth % 8) != 0) {
			info->ImageWidth += (8 - (info->ImageWidth % 8));
		}

		if ((info->ImageHeight % 8) != 0) {
			info->ImageHeight += (8 - (info->ImageHeight % 8));
		}
	}

	if (JPEG_GetDecodeColorConvertFunc(info, &convertMCUBlocks, &_mcu_number) != HAL_OK) {
		// TODO(@yann): handle errors
	}
}

void CoreJPEGModeDMA::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO(@yann): handle errors
}

void CoreJPEGModeDMA::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t output_datasize)
{
	auto &write_buffer		= _output_buffers.at(_output_buffers_write_index);
	auto &next_write_buffer = _output_buffers.at((_output_buffers_write_index + 1) % jpeg::output_buffers_nb);

	write_buffer.state	  = Buffer::State::Full;
	write_buffer.datasize = output_datasize;

	_output_buffers_write_index = (_output_buffers_write_index + 1) % jpeg::output_buffers_nb;

	if (next_write_buffer.state != Buffer::State::Empty) {
		_out_paused = true;
		_hal.HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}
	_hal.HAL_JPEG_ConfigOutputBuffer(hjpeg, next_write_buffer.data, jpeg::output_chunk_size);
}

void CoreJPEGModeDMA::onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize)
{
	auto &read_buffer	   = _input_buffers.at(_input_buffers_read_index);
	auto &next_read_buffer = _input_buffers.at((_input_buffers_read_index + 1) % jpeg::input_buffers_nb);

	if (decoded_datasize == read_buffer.datasize) {
		read_buffer.state	 = Buffer::State::Empty;
		read_buffer.datasize = 0;

		_input_buffers_read_index = (_input_buffers_read_index + 1) % jpeg::input_buffers_nb;

		if (next_read_buffer.state == Buffer::State::Empty) {
			_in_paused = true;
			_hal.HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		} else {
			_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data, next_read_buffer.datasize);
		}
	} else {
		_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data + decoded_datasize,
										next_read_buffer.datasize - decoded_datasize);
	}
	_image_size += decoded_datasize;
}

void CoreJPEGModeDMA::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	_hw_decode_ended = true;
}

auto CoreJPEGModeDMA::decode(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> size_t
{
	reset();

	// read file and fill input buffers
	for (auto &buffer: _input_buffers) {
		buffer.datasize = file.read(buffer.data, jpeg::input_chunk_size);
		if (buffer.datasize > 0) {
			buffer.state = Buffer::State::Full;
		}
	}

	// start JPEG decoding with DMA method
	_hal.HAL_JPEG_Decode_DMA(hjpeg, _input_buffers[0].data, _input_buffers[0].datasize, _output_buffers[0].data,
							 jpeg::output_chunk_size);

	// loop until decode process ends
	auto process_ended = bool {false};
	do {
		decoderInputHandler(hjpeg, file);
		process_ended = decoderOutputHandler(hjpeg);
	} while (!process_ended);

	return _image_size;
}

std::array<uint8_t, jpeg::input_chunk_size * jpeg::input_buffers_nb> CoreJPEGModeDMA::BIG_CHUNGUS_OF_MEMORY_IN;
std::array<uint8_t, jpeg::output_chunk_size * jpeg::output_buffers_nb> CoreJPEGModeDMA::BIG_CHUNGUS_OF_MEMORY_OUT;

void CoreJPEGModeDMA::reset()
{
	_image_size = 0;

	_mcu_number		 = 0;
	_mcu_block_index = 0;
	_hw_decode_ended = false;

	_input_buffers_read_index	= 0;
	_input_buffers_write_index	= 0;
	_output_buffers_read_index	= 0;
	_output_buffers_write_index = 0;

	_in_paused	= false;
	_out_paused = false;

	// initialize memory chungus
	uint32_t i = 0;
	for (auto &buffer: _input_buffers) {
		buffer.state	= Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data		= BIG_CHUNGUS_OF_MEMORY_IN.data() + i * jpeg::input_chunk_size;
		i += 1;
	}
	i = 0;
	for (auto &buffer: _output_buffers) {
		buffer.state	= Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data		= BIG_CHUNGUS_OF_MEMORY_OUT.data() + i * jpeg::output_chunk_size;
		i += 1;
	}
}

void CoreJPEGModeDMA::decoderInputHandler(JPEG_HandleTypeDef *hjpeg, interface::File &file)
{
	auto &write_buffer = _input_buffers[_input_buffers_write_index];
	auto &read_buffer  = _input_buffers[_input_buffers_read_index];

	if (write_buffer.state == Buffer::State::Empty) {
		write_buffer.datasize = file.read(write_buffer.data, jpeg::input_chunk_size);
		if (write_buffer.datasize > 0) {
			write_buffer.state = Buffer::State::Full;
		} else {
			// TODO(@yann): handle errors
		}

		if (_in_paused && _input_buffers_write_index == _input_buffers_read_index) {
			_in_paused = false;
			_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, read_buffer.data, read_buffer.datasize);
			_hal.HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		}

		_input_buffers_write_index = (_input_buffers_write_index + 1) % jpeg::input_buffers_nb;
	}
}

auto CoreJPEGModeDMA::decoderOutputHandler(JPEG_HandleTypeDef *hjpeg) -> bool
{
	auto converted_data_count = uint32_t {0};
	auto &read_buffer		  = _output_buffers.at(_output_buffers_read_index);
	const auto &write_buffer  = _output_buffers.at(_output_buffers_write_index);

	if (read_buffer.state == Buffer::State::Full) {
		_mcu_block_index += convertMCUBlocks(read_buffer.data, (uint8_t *)jpeg::decoded_buffer_address,
											 _mcu_block_index, read_buffer.datasize, &converted_data_count);

		read_buffer.state	 = Buffer::State::Empty;
		read_buffer.datasize = 0;

		_output_buffers_read_index = (_output_buffers_read_index + 1) % jpeg::output_buffers_nb;
	} else if (_out_paused && write_buffer.state == Buffer::State::Empty && read_buffer.state == Buffer::State::Empty) {
		_out_paused = false;
		_hal.HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}

	return _mcu_block_index == _mcu_number;
}
