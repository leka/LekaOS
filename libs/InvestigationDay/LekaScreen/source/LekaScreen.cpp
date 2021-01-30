// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

leka::LKScreen _screen(otm8009a_model);
leka::Display display(_screen);

Screen::Screen() : _interface(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK), _file_interface("leka_fs"), _sd_enable(SD_SPI_CS)
{
}

void Screen::squareBouncing()
{
	uint32_t posx  = 0;
	uint32_t posy  = 0;
	uint32_t dirx  = 1;
	uint32_t diry  = 1;
	uint32_t sizex = 100;
	uint32_t sizey = 100;

	uint32_t bg_color = 0xffffff00;

	// square color
	uint8_t alpha = 0xff;
	uint8_t red	  = 0xff;
	uint8_t green = 0x00;
	uint8_t blue  = 0x00;

	// initialize and select layer 0
	LTDCLayerInit();
	clear(bg_color);

	while (true) {
		// update position
		posx = (posx + dirx);
		posy = (posy + diry);

		// chek for screen limits
		if (posx >= 800 - sizex || posx == 0) {
			dirx *= -1;
		}
		if (posy >= 480 - sizey || posy == 0) {
			diry *= -1;
		}

		// draw the square
		drawRectangle(posx, posy, sizex, sizey, (alpha << 24) | (red << 16) | (green << 8) | (blue));
		// update colors
		if (green == 0) {
			red--;
			blue++;
		}
		if (red == 0) {
			green++;
			blue--;
		}
		if (blue == 0) {
			red++;
			green--;
		}

		rtos::ThisThread::sleep_for(1ms);
	}
}

int Screen::SDInit()
{
	if (0 != _interface.init()) {
		return -1;
	}

	if (0 != _interface.frequency(5000000)) {
		printf("Error setting frequency \n");
	}

	return 0;
}

void Screen::LTDCLayerInit()
{
	LTDC_LayerCfgTypeDef Layercfg;

	/* Layer Init */
	Layercfg.WindowX0		 = 0;
	Layercfg.WindowX1		 = display.getWidth();
	Layercfg.WindowY0		 = 0;
	Layercfg.WindowY1		 = display.getHeight();
	Layercfg.PixelFormat	 = LTDC_PIXEL_FORMAT_ARGB8888;
	Layercfg.FBStartAdress	 = LCD_FRAME_BUFFER;   // Previously FB_Address given in parameter
	Layercfg.Alpha			 = 255;
	Layercfg.Alpha0			 = 0;
	Layercfg.Backcolor.Blue	 = 0;
	Layercfg.Backcolor.Green = 0;
	Layercfg.Backcolor.Red	 = 0;
	Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	Layercfg.ImageWidth		 = display.getWidth();
	Layercfg.ImageHeight	 = display.getHeight();

	HAL_LTDC_ConfigLayer(&leka::hltdc, &Layercfg, 1);

	// DrawProp[1].BackColor = LCD_COLOR_WHITE;
	// DrawProp[1].pFont	   = &Font24;
	// DrawProp[1].TextColor = LCD_COLOR_BLACK;
}

void Screen::clear(uint32_t ColorIndex)
{
	leka::DMA2DDrawing((uint32_t *)(leka::hltdc.LayerCfg[1].FBStartAdress), display.getWidth(), display.getHeight(), 0,
					   ColorIndex);
}

void Screen::drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex)
{
	uint32_t Xaddress = 0;

	/* Set the text color */
	// BSP_LCD_SetTextColor(DrawProp[1].TextColor);

	/* Get the rectangle start address */
	Xaddress		= (leka::hltdc.LayerCfg[1].FBStartAdress) + 4 * (display.getWidth() * Ypos + Xpos);
	uint32_t offset = display.getWidth() - Width;

	/* Fill the rectangle */
	leka::DMA2DDrawing((uint32_t *)Xaddress, Width, Height, offset, ColorIndex);
}

void Screen::showFace(bool jpeg_file)
{
	if (jpeg_file) {
		static char filename[] = "assets/images/emotion-happy.jpg";
		FIL JPEG_File; /* File object */

		uint32_t width_offset = 0;

		printf("\n\r--------Programm starting--------\n\r");

		/*##-1- JPEG Initialization ################################################*/
		LTDCLayerInit();

		uint32_t bg_color = 0xffffff00;
		clear(bg_color);

		//##-4- Register the file system object to the FatFs module ##############
		_file_interface.mount(&_interface);
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
		LTDCLayerInit();
		clear(bg_color);
	}
	return;
}

void Screen::start()
{
	printf("Screen example\n\n");
	SDInit();

	display.Init();

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
		squareBouncing();
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}
