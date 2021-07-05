// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_WAVE_PLAYER_SETUP_H_
#define _LEKA_OS_SPIKE_WAVE_PLAYER_SETUP_H_

#include "mbed.h"
//#include "mbed_wait_api.h"
//#include <cmath>
//#include <cstdio>
//#include <cstdint>

//#include "stm32f7xx_hal.h"
//#include "stm32f7xx_hal_dma.h"

#define BYTES_IN_BUFFER		 1024
#define BYTES_PER_SAMPLE	 2
#define SAMPLES_IN_BUFF		 (BYTES_IN_BUFFER / BYTES_PER_SAMPLE)
#define SAMPLES_IN_HALF_BUFF (SAMPLES_IN_BUFF / 2)

enum DMA1_IRQ_Flag
{
	None,
	DMA1_half_cpt,
	DMA1_cpt
};

enum SamplingRate{
	Sampling_11K = 11025,
	Sampling_22K = 22050,
	Sampling_44K = 44100,
	Sampling_48K = 48000,
	Sampling_96K = 96000
};

/* Private variables ---------------------------------------------------------*/

mbed::DigitalOut digiOut(WIFI_RESET);

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
TIM_HandleTypeDef htim6;
DMA1_IRQ_Flag _flag = None;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Init();
void DMA_Init();
void TIM6_Init(SamplingRate sRate);
void DAC_Init();

/* Prototypes ----------------------------------------------------------------*/
void init_TIM_DAC_DMA();
void start_TIM_DAC_DMA(uint32_t *ptrData);

void Error_Handler();
extern "C" {
void DMA1_Stream5_IRQHandler();
}
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac);
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac);
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base);

//########################### Private functions #########################################
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init()
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

/**
 * Enable DMA controller clock
 */
void DMA_Init()
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
void TIM6_Init(SamplingRate sRate)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	uint32_t clockFreq = HAL_RCC_GetPCLK1Freq();
 
	/* Get PCLK1 prescaler */
	if((RCC->CFGR & RCC_CFGR_PPRE1) != 0)
	{
		clockFreq *=2;
	};

	uint32_t period = 0;

	//period = ((double)clockFreq / sr)-1;
	period = clockFreq /sRate ;
	// printf("First period calculation %u\n",period);

	// switch (sRate) {
	// case Sampling_11K:
	// 	period = 9795;
	// 	break;
	// case Sampling_22K:
	// 	period = 4897;
	// 	break;
	// case Sampling_44K:
	// 	period = 2448;
	// 	break;
	// case Sampling_48K:
	// 	period = 2250;
	// 	break;
	// case Sampling_96K:
	// 	period = 1125;
	// 	break;
	// }
	// printf("Second period calculation %u\n",period);
	

	htim6.Instance				 = TIM6;
	htim6.Init.Prescaler		 = 0;
	htim6.Init.CounterMode		 = TIM_COUNTERMODE_UP;
	htim6.Init.Period			 = period;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
void DAC_Init()
{
	DAC_ChannelConfTypeDef sConfig = {0};

	/** DAC Initialization
	 */
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		Error_Handler();
	}
	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger		 = DAC_TRIGGER_T6_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;	  // TODO : check if need to enable or not
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
}

void init_TIM_DAC_DMA()
{
	GPIO_Init();
	DMA_Init();
	TIM6_Init(SamplingRate::Sampling_44K);   // TODO add period change with sample rate
	DAC_Init();
}

void start_TIM_DAC_DMA(uint32_t *ptrData)
{
	HAL_TIM_Base_Start(&htim6);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, ptrData, SAMPLES_IN_BUFF, DAC_ALIGN_12B_R);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None//TODO : handle errors if necessary
 */
void Error_Handler()
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (true) {
		printf("Error in config");
	}
	/* USER CODE END Error_Handler_Debug */
}

//########################### Overwritten functions #########################################

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
	void DMA1_Stream5_IRQHandler()
	{
		HAL_DMA_IRQHandler(&hdma_dac1);
	}
}


void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	_flag = DMA1_cpt;
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	_flag = DMA1_half_cpt;
}

/**
 * @brief DAC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (hdac->Instance == DAC) {
		/* Peripheral clock enable */
		__HAL_RCC_DAC_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**DAC GPIO Configuration
		PA4     ------> DAC_OUT1
		*/
		GPIO_InitStruct.Pin	 = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* DAC DMA Init */
		/* DAC1 Init */
		hdma_dac1.Instance				   = DMA1_Stream5;
		hdma_dac1.Init.Channel			   = DMA_CHANNEL_7;
		hdma_dac1.Init.Direction		   = DMA_MEMORY_TO_PERIPH;
		hdma_dac1.Init.PeriphInc		   = DMA_PINC_DISABLE;
		hdma_dac1.Init.MemInc			   = DMA_MINC_ENABLE;
		hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_dac1.Init.MemDataAlignment	   = DMA_MDATAALIGN_HALFWORD;
		hdma_dac1.Init.Mode				   = DMA_CIRCULAR;
		hdma_dac1.Init.Priority			   = DMA_PRIORITY_LOW;		// changed from low to high
		hdma_dac1.Init.FIFOMode			   = DMA_FIFOMODE_ENABLE;	// enable
		hdma_dac1.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_dac1.Init.MemBurst			   = DMA_MBURST_INC4;	// inc4
		hdma_dac1.Init.PeriphBurst		   = DMA_PBURST_SINGLE;

		if (HAL_DMA_Init(&hdma_dac1) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1);
	}
}

/**
 * @brief DAC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
	//printf("HAL_DAC_MspDeInit\n");
	if (hdac->Instance == DAC) {
		/* Peripheral clock disable */
		__HAL_RCC_DAC_CLK_DISABLE();

		/**DAC GPIO Configuration
		PA4     ------> DAC_OUT1
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

		/* DAC DMA DeInit */
		HAL_DMA_DeInit(hdac->DMA_Handle1);
	}
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
	//printf("HAL_TIM_Base_MspInit\n");
	if (htim_base->Instance == TIM6) {
		/* Peripheral clock enable */
		
		__HAL_RCC_TIM6_CLK_ENABLE();
	}
}

/**
 * @brief TIM_Base MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
	//printf("HAL_TIM_Base_MspDeInit\n");
	if (htim_base->Instance == TIM6) {
		/* Peripheral clock disable */
		__HAL_RCC_TIM6_CLK_DISABLE();
	}
}


#endif	 // _LEKA_OS_SPIKE_WAVE_PLAYER_SETUP_H_