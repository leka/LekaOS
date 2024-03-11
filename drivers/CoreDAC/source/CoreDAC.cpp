// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDAC.h"

using namespace leka;

CoreDAC::CoreDAC(interface::STM32Hal &hal, interface::STM32HalBasicTimer &hal_timer) : _hal(hal), _hal_timer(hal_timer)
{
	_hdac.Instance = DAC;
}

auto CoreDAC::getHandle() -> DAC_HandleTypeDef &
{
	return _hdac;
}

void CoreDAC::initialize()
{
	_registerMspCallbacks();

	_hal.HAL_DAC_Init(&_hdac);

	DAC_ChannelConfTypeDef config = {};
	config.DAC_OutputBuffer		  = DAC_OUTPUTBUFFER_ENABLE;
	_hal_timer.linkDACTimer(&config);
	_hal.HAL_DAC_ConfigChannel(&_hdac, &config, DAC_CHANNEL_1);

	static const auto &self = *this;
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_HALF_COMPLETE_CB_ID,
								  []([[maybe_unused]] DAC_HandleTypeDef *hdac) {
									  if (self._on_half_transfer != nullptr) {
										  self._on_half_transfer();
									  }
								  });
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_COMPLETE_CB_ID, []([[maybe_unused]] DAC_HandleTypeDef *hdac) {
		if (self._on_complete_transfer != nullptr) {
			self._on_complete_transfer();
		}
	});
}

void CoreDAC::terminate()
{
	_hal.HAL_DAC_DeInit(&_hdac);
}

void CoreDAC::registerDataToPlay(std::span<uint16_t> data)
{
	_data = data;
}

void CoreDAC::registerDMACallbacks(std::function<void()> const &on_half_transfer,
								   std::function<void()> const &on_complete_transfer)
{
	_on_half_transfer	  = on_half_transfer;
	_on_complete_transfer = on_complete_transfer;
}

void CoreDAC::start()
{
	_hal_timer.start();
	_hal.HAL_DAC_Start_DMA(&_hdac, DAC_CHANNEL_1, reinterpret_cast<uint32_t *>(_data.data()), _data.size(),
						   DAC_ALIGN_12B_R);
}

void CoreDAC::stop()
{
	_hal.HAL_DAC_Stop_DMA(&_hdac, DAC_CHANNEL_1);
}

void CoreDAC::_registerMspCallbacks()
{
	static auto &self = *this;

	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSPINIT_CB_ID, []([[maybe_unused]] DAC_HandleTypeDef *hdac) {
		__HAL_LINKDMA(&self._hdac, DMA_Handle1, self._hdma);
		self._initializeDMA();

		self._hal.HAL_RCC_DAC_CLK_ENABLE();
	});

	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSPDEINIT_CB_ID, []([[maybe_unused]] DAC_HandleTypeDef *hdac) {
		self._hal.HAL_DMA_DeInit(&self._hdma);

		self._hal.HAL_RCC_DAC_CLK_DISABLE();
	});
}

void CoreDAC::_initializeDMA()
{
	_hal.HAL_RCC_DMA1_CLK_ENABLE();

	_hal.HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	_hdma.Instance = DMA1_Stream5;	 // DMA1_Stream5 is the only DMA channel for DAC

	_hdma.Init.Channel			   = DMA_CHANNEL_7;
	_hdma.Init.Direction		   = DMA_MEMORY_TO_PERIPH;
	_hdma.Init.PeriphInc		   = DMA_PINC_DISABLE;
	_hdma.Init.MemInc			   = DMA_MINC_ENABLE;
	_hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	_hdma.Init.MemDataAlignment	   = DMA_MDATAALIGN_HALFWORD;
	_hdma.Init.Mode				   = DMA_CIRCULAR;
	_hdma.Init.Priority			   = DMA_PRIORITY_LOW;
	_hdma.Init.FIFOMode			   = DMA_FIFOMODE_ENABLE;
	_hdma.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_HALFFULL;
	_hdma.Init.MemBurst			   = DMA_MBURST_SINGLE;
	// Single mem burst is more ressource consuming than 4 burst or more
	// However the buffer apparently needs to be of a size multiple of the burst mode chosen
	_hdma.Init.PeriphBurst = DMA_PBURST_SINGLE;

	_hal.HAL_DMA_Init(&_hdma);
}
