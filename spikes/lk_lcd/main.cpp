// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "FileManager.h"
#include "HelloWorld.h"
#include "LKCoreDMA2D.h"
#include "LKCoreDSI.h"
#include "LKCoreJPEG.h"
#include "LKCoreLCD.h"
#include "LKCoreLTDC.h"
#include "LKCoreSTM32Hal.h"
#include "st_otm8009a.h"
#include "st_sdram.h"

using namespace leka;

HelloWorld hello;

LKCoreSTM32Hal hal;
LKCoreDMA2D coredma2d(hal);
LKCoreDSI coredsi(hal);
LKCoreJPEG corejpeg(coredma2d);
LKCoreLCD corelcd(SCREEN_BACKLIGHT_PWM, 0.5f);
LKCoreLTDC coreltdc(hal, coredsi);
FileManager sd_card;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread screen_thread;

void init()
{
	DSI_IO_RegisterWriteCmd(
		[](uint32_t NbrParams, uint8_t *pParams)
		{
			coredsi.write(pParams, NbrParams);
		});

	coredsi.reset();

	/** @brief Enable the LTDC clock */
	__HAL_RCC_LTDC_CLK_ENABLE();

	/** @brief Toggle Sw reset of LTDC IP */
	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_LTDC_RELEASE_RESET();

	/** @brief Enable the DMA2D clock */
	__HAL_RCC_DMA2D_CLK_ENABLE();

	/** @brief Toggle Sw reset of DMA2D IP */
	__HAL_RCC_DMA2D_FORCE_RESET();
	__HAL_RCC_DMA2D_RELEASE_RESET();

	/** @brief Enable DSI Host and wrapper clocks */
	__HAL_RCC_DSI_CLK_ENABLE();

	/** @brief Soft Reset the DSI Host and wrapper */
	__HAL_RCC_DSI_FORCE_RESET();
	__HAL_RCC_DSI_RELEASE_RESET();

	/** @brief NVIC configuration for LTDC interrupt that is now enabled */
	HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);

	/** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	/** @brief NVIC configuration for DSI interrupt that is now enabled */
	HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DSI_IRQn);

	coredsi.initialize();

	coreltdc.initialize();
	coreltdc.configureLayer();

	coredsi.start();

	BSP_SDRAM_Init();

	corelcd.initialize();
	corejpeg.initialize();
	coredma2d.initialize();

	HAL_JPEG_RegisterInfoReadyCallback(corejpeg.getHandlePointer(),
									   [](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
									   {
										   corejpeg.onInfoReadyCallback(hjpeg, info);
									   });

	HAL_JPEG_RegisterGetDataCallback(corejpeg.getHandlePointer(),
									 [](JPEG_HandleTypeDef *hjpeg, uint32_t size)
									 {
										 corejpeg.onDataAvailableCallback(hjpeg, size);
									 });

	HAL_JPEG_RegisterDataReadyCallback(corejpeg.getHandlePointer(),
									   [](JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size)
									   {
										   corejpeg.onDataReadyCallback(hjpeg, pDataOut, size);
									   });

	HAL_JPEG_RegisterCallback(corejpeg.getHandlePointer(), HAL_JPEG_DECODE_CPLT_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg)
							  {
								  corejpeg.onDecodeCompleteCallback(hjpeg);
							  });

	HAL_JPEG_RegisterCallback(corejpeg.getHandlePointer(), HAL_JPEG_ERROR_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg)
							  {
								  corejpeg.onErrorCallback(hjpeg);
							  });
}

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	init();

	char filename1[] = "assets/images/Leka/logo.jpg";
	char filename2[] = "assets/images/Leka/emotion-happy.jpg";

	FIL JPEG_File;

	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);

		if (f_open(&JPEG_File, filename1, FA_READ) == FR_OK) {
			corejpeg.displayImage(&JPEG_File);
			f_close(&JPEG_File);
			rtos::ThisThread::sleep_for(2s);
		}

		if (f_open(&JPEG_File, filename2, FA_READ) == FR_OK) {
			corejpeg.displayImage(&JPEG_File);
			f_close(&JPEG_File);
			rtos::ThisThread::sleep_for(2s);
		}
	}

	return 0;
}
