// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

leka::LKScreen _screen(otm8009a_model);
leka::Display display(_screen);

Screen::Screen() {}

void Screen::clear(uint32_t ColorIndex)
{
	leka::drawClear(ColorIndex);
}

void Screen::showFace(bool jpeg_file)
{
	if (jpeg_file) {
		static char filename[] = "assets/images/emotion-happy.jpg";
		FIL JPEG_File; /* File object */

		uint32_t width_offset = 0;

		printf("\n\r--------Programm starting--------\n\r");

		uint32_t bg_color = 0xffffff00;
		clear(bg_color);

		//##-5- Open the JPG file with read access #############################
		if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
			printf("File %s openened. File size : %lu \n\r", filename, f_size(&JPEG_File));

			// JPEG_Decode_DMA(&hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);
			JPEG_DecodePolling(&hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);

			HAL_JPEG_GetInfo(&hjpeg, &hjpeginfo);

			uint16_t xPos = (display.getWidth() - hjpeginfo.ImageWidth) / 2;
			uint16_t yPos = (display.getHeight() - hjpeginfo.ImageHeight) / 2;
			if (hjpeginfo.ChromaSubsampling == JPEG_420_SUBSAMPLING) {
				if ((hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (hjpeginfo.ImageWidth % 16);
			}

			if (hjpeginfo.ChromaSubsampling == JPEG_422_SUBSAMPLING) {
				if ((hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (hjpeginfo.ImageWidth % 16);
			}

			if (hjpeginfo.ChromaSubsampling == JPEG_444_SUBSAMPLING) {
				if ((hjpeginfo.ImageWidth % 8) != 0) width_offset = (hjpeginfo.ImageWidth % 8);
			}

			leka::DMA2DImage((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos, yPos,
							 hjpeginfo.ImageWidth, hjpeginfo.ImageHeight, width_offset);

			rtos::ThisThread::sleep_for(1s);
			//##-10- Close the avi file ##########################################
			f_close(&JPEG_File);

		} else
			printf("Failed to open file %s \n\r", filename);
	} else {
		uint32_t bg_color = 0xffffffff;
		clear(bg_color);
	}
	return;
}

void Screen::start()
{
	printf("Screen example\n\n");

	display.Init();
	leka::drawingToolsInit(otm8009a_model);

	showFace(true);
	// rtos::ThisThread::sleep_for(30s);
	/* TO DELETE */
	for (int i = 0; i < 21; i++) {
		if (i % 2) {
			display.turnOff();
			display.rotateUpsideDown(true);
		} else {
			display.turnOn();
			display.rotateUpsideDown(false);
		}
		display.setBrightness((float)(i / 21.0f));
		rtos::ThisThread::sleep_for(1s);
	}
	display.setBrightness(0.3f);
	/* END OF TO DELETE */

	while (true) {
		leka::screenSaver();
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}
