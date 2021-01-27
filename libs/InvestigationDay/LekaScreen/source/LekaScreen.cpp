// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d);
void DMA2D_TransferErrorCallback(DMA2D_HandleTypeDef *hdma2d);
uint32_t JPEG_FindFrameOffset(uint32_t offset, FIL *file);

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
	LTDCLayerInit(0);
	setActiveLayer(0);
	// clear layer 0 in yellow
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

		// HAL_Delay(1);	// ~2ms little delay to let things settle
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

void Screen::LTDCLayerInit(uint16_t layer_index)
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

	HAL_LTDC_ConfigLayer(&leka::hltdc, &Layercfg, layer_index);

	// DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
	// DrawProp[LayerIndex].pFont	   = &Font24;
	// DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

void Screen::setActiveLayer(uint32_t layer_index)
{
	_active_layer = layer_index;
}

void Screen::clear(uint32_t ColorIndex)
{
	fillBuffer(_active_layer, (uint32_t *)(leka::hltdc.LayerCfg[_active_layer].FBStartAdress), display.getWidth(),
			   display.getHeight(), 0, ColorIndex);
}

void Screen::drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex)
{
	uint32_t Xaddress = 0;

	/* Set the text color */
	// BSP_LCD_SetTextColor(DrawProp[_active_layer].TextColor);

	/* Get the rectangle start address */
	Xaddress		= (leka::hltdc.LayerCfg[_active_layer].FBStartAdress) + 4 * (display.getWidth() * Ypos + Xpos);
	uint32_t offset = display.getWidth() - Width;

	/* Fill the rectangle */
	fillBuffer(_active_layer, (uint32_t *)Xaddress, Width, Height, offset, ColorIndex);
}

void Screen::drawImage(uint32_t data, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	uint32_t destination = LCD_FRAME_BUFFER + (x + y * display.getWidth()) * 4;
	_hdma2d.Instance	 = DMA2D;

	_hdma2d.Init.Mode		  = DMA2D_M2M_BLEND;
	_hdma2d.Init.ColorMode	  = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset = display.getWidth() - width;

	// Foreground
	_hdma2d.LayerCfg[1].AlphaMode	   = DMA2D_NO_MODIF_ALPHA;
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	_hdma2d.LayerCfg[1].InputOffset	   = 0;
	_hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA;

	// Background
	_hdma2d.LayerCfg[0].AlphaMode	   = DMA2D_NO_MODIF_ALPHA;
	_hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
	_hdma2d.LayerCfg[0].InputOffset	   = display.getWidth() - width;

	HAL_DMA2D_Init(&_hdma2d);
	HAL_DMA2D_ConfigLayer(&_hdma2d, 1);
	HAL_DMA2D_ConfigLayer(&_hdma2d, 0);
	// HAL_DMA2D_Start(&_hdma2d, data, destination, width, height);
	HAL_DMA2D_BlendingStart(&_hdma2d, data, destination, destination, width, height);
	HAL_DMA2D_PollForTransfer(&_hdma2d, 10);
}

void Screen::fillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine,
						uint32_t ColorIndex)
{
	/* Register to memory mode with ARGB8888 as color Mode */
	_hdma2d.Init.Mode		  = DMA2D_R2M;
	_hdma2d.Init.ColorMode	  = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset = OffLine;

	_hdma2d.Instance = DMA2D;

	/* DMA2D Initialization */
	if (HAL_DMA2D_Init(&_hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&_hdma2d, LayerIndex) == HAL_OK) {
			if (HAL_DMA2D_Start(&_hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK) {
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&_hdma2d, 10);
			}
		}
	}
}

int DMA2D_IRQ_counter = 0;
int HAL_error_status  = 0;

void Screen::showFace(bool jpeg_file)
{
	if (jpeg_file) {
		static char filename[] = "assets/images/emotion-happy.jpg";
		// static char filename[] = "video.avi";
		FIL JPEG_File; /* File object */

		// static uint32_t FrameOffset = 0;
		// uint32_t JpegProcessing_End = 0;
		uint32_t width_offset = 0;

		leka::DSI_IRQ_counter = 0;
		printf("\n\r--------Programm starting--------\n\r");

		// uint32_t isfirstFrame, currentFrameRate;
		// auto startTime = HAL_GetTick();
		// auto endTime   = HAL_GetTick();
		// char message[16];

		/*##-1- JPEG Initialization ################################################*/
		// LCD_LayerInit(0, LCD_FB_START_ADDRESS);
		// BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
		LTDCLayerInit(0);	// FLAG for passing address
		// BSP_LCD_SelectLayer(0);
		setActiveLayer(0);
		printf("LCD_LayerInit done \n\r");

		/*HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
		HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);
		// Update pitch : the draw is done on the whole physical X Size
		HAL_LTDC_SetPitch(&hltdc_discovery, OTM8009A_800X480_WIDTH, 0);*/

		// pending_buffer = 0;
		// active_area = LEFT_AREA;

		// BSP_LCD_Clear(LCD_COLOR_CYAN);
		uint32_t bg_color = 0xffffff00;
		clear(bg_color);
		// clear layer 0 in yellow

		printf("Screen Clear done \n\r");

		// HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);

		// Send Display On DCS Command to display
		HAL_DSI_ShortWrite(&(leka::hdsi), 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON, 0x00);
		// HAL_DSI_Refresh(&hdsi_discovery);

		// printf("DSI IRQ calls : %d \n\r \n\r", DSI_IRQ_counter);
		//##-4- Register the file system object to the FatFs module ##############
		_file_interface.mount(&_interface);
		//##-5- Open the JPG file with read access #############################
		if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
			printf("File %s openened. File size : %lu \n\r", filename, f_size(&JPEG_File));

			printf("Before Decoding\n");
			// JPEG_Decode_DMA(&hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);
			JPEG_DecodePolling(&hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);
			printf("After decoding\n");

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

			DMA2D_CopyBuffer((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos, yPos,
							 hjpeginfo.ImageWidth, hjpeginfo.ImageHeight, width_offset);
			printf("End of CopyBuffer\n");
			rtos::ThisThread::sleep_for(1s);
			//##-10- Close the avi file ##########################################
			f_close(&JPEG_File);

		} else
			printf("Failed to open file %s \n\r", filename);
		// printf("Frame offset %lu \n\r", FrameOffset);
	} else {
		uint32_t bg_color = 0xffffffff;
		// initialize and select layer 0
		LTDCLayerInit(0);
		// lcd.LTDCLayerInit(1);
		setActiveLayer(0);
		// clear layer 0 in white
		clear(bg_color);
	}
	return;
}

#define PATTERN_SEARCH_BUFFERSIZE 512
/* Private macro -------------------------------------------------------------*/
#define __DSI_MASK_TE()	  (GPIOJ->AFR[0] &= (0xFFFFF0FFU)) /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/

#define JPEG_SOI_MARKER		  (0xFFD8) /* JPEG Start Of Image marker*/
#define JPEG_SOI_MARKER_BYTE0 (JPEG_SOI_MARKER & 0xFF)
#define JPEG_SOI_MARKER_BYTE1 ((JPEG_SOI_MARKER >> 8) & 0xFF)
/* Private variables ---------------------------------------------------------*/
static uint8_t PatternSearchBuffer[PATTERN_SEARCH_BUFFERSIZE];

/**
 * @brief  Initialize the DMA2D in memory to memory with PFC.
 * @param  ImageWidth: image width
 * @param  ImageHeight: image Height
 * @retval None
 */
void Screen::DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight)
{
	/* Init DMA2D */
	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	_hdma2d.Init.Mode		   = DMA2D_M2M_PFC;	  // memory to memory with pixel format convert
	_hdma2d.Init.ColorMode	   = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset  = display.getWidth() - ImageWidth;
	_hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Output Alpha Inversion*/
	_hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;	  /* No Output Red & Blue swap */

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	_hdma2d.XferCpltCallback  = DMA2D_TransferCompleteCallback;
	_hdma2d.XferErrorCallback = DMA2D_TransferErrorCallback;

	/*##-3- Foreground Configuration ###########################################*/
	_hdma2d.LayerCfg[1].AlphaMode	   = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[1].InputAlpha	   = 0xFF;
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	_hdma2d.LayerCfg[1].InputOffset	   = 0;
	_hdma2d.LayerCfg[1].RedBlueSwap	   = DMA2D_RB_REGULAR;	  /* No ForeGround Red/Blue swap */
	_hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

	_hdma2d.Instance = DMA2D;

	/*##-4- DMA2D Initialization     ###########################################*/
	HAL_DMA2D_Init(&_hdma2d);
	HAL_DMA2D_ConfigLayer(&_hdma2d, 1);
}

/**
 * @brief  Copy the Decoded image to the display Frame buffer.
 * @param  pSrc: Pointer to source buffer
 * @param  pDst: Pointer to destination buffer
 * @param  ImageWidth: image width
 * @param  ImageHeight: image Height
 * @retval None
 */
void Screen::DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,
							  uint32_t width_offset)
{
	// uint32_t x			 = (display.getWidth() - hjpeginfo.ImageWidth) / 2;
	// uint32_t y			 = (display.getHeight() - hjpeginfo.ImageHeight) / 2;
	// uint32_t destination = (uint32_t)pDst + (y * display.getWidth() + x) * 4;
	// // printf("DMA copy buffer \n\r");
	// /*while (pending_buffer != -1) {
	// }*/
	// HAL_DMA2D_Start(&_hdma2d, (uint32_t)pSrc, destination, ImageWidth, ImageHeight);
	// HAL_DMA2D_PollForTransfer(&_hdma2d, 100);

	uint32_t destination = (uint32_t)pDst + (y * display.getWidth() + x) * 4;
	uint32_t source		 = (uint32_t)pSrc;

	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	_hdma2d.Init.Mode		   = DMA2D_M2M_PFC;
	_hdma2d.Init.ColorMode	   = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset  = display.getWidth() - xsize;
	_hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Output Alpha Inversion*/
	_hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;	  /* No Output Red & Blue swap */

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	_hdma2d.XferCpltCallback = NULL;

	/*##-3- Foreground Configuration ###########################################*/
	_hdma2d.LayerCfg[1].AlphaMode  = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[1].InputAlpha = 0xFF;

#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;

#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;

#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;

#endif /* JPEG_RGB_FORMAT * */

	_hdma2d.LayerCfg[1].InputOffset	  = width_offset;
	_hdma2d.LayerCfg[1].RedBlueSwap	  = DMA2D_RB_REGULAR;	 /* No ForeGround Red/Blue swap */
	_hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

	_hdma2d.Instance = DMA2D;

	/* DMA2D Initialization */
	if (HAL_DMA2D_Init(&_hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&_hdma2d, 1) == HAL_OK) {
			if (HAL_DMA2D_Start(&_hdma2d, source, destination, xsize, ysize) == HAL_OK) {
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&_hdma2d, 100);
			}
		}
	}
}

/**
 * @brief  DMA2D Transfer completed callback
 * @param  hdma2d: DMA2D handle.
 * @retval None
 */
void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d)
{
	// Frame Buffer updated , unmask the DSI TE pin to ask for a DSI refersh
	/*pending_buffer = 1;*/
	// UnMask the TE
	//__DSI_UNMASK_TE();
	// HAL_DSI_Refresh(&hdsi_discovery);
}

void DMA2D_TransferErrorCallback(DMA2D_HandleTypeDef *hdma2d)
{
	OnError_Handler();
}

/**
 * @brief  Find Next JPEG frame offset in the video file.
 * @param  offset: Current offset in the video file.
 * @param  file: Pointer to the video file object.
 * @retval None
 */
uint32_t JPEG_FindFrameOffset(uint32_t offset, FIL *file)
{
	uint32_t index = offset, i, readSize = 0;
	do {
		if (f_size(file) <= (index + 1)) {
			/* end of file reached*/
			return 0;
		}
		f_lseek(file, index);
		f_read(file, PatternSearchBuffer, PATTERN_SEARCH_BUFFERSIZE, (UINT *)(&readSize));
		if (readSize != 0) {
			for (i = 0; i < (readSize - 1); i++) {
				if ((PatternSearchBuffer[i] == JPEG_SOI_MARKER_BYTE1) &&
					(PatternSearchBuffer[i + 1] == JPEG_SOI_MARKER_BYTE0)) {
					return index + i;
				}
			}

			index += (readSize - 1);
		}
	} while (readSize != 0);

	return 0;
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
	/* END OF TO DELETE */

	while (true) {
		squareBouncing();
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}

void Screen::DMA2D_IRQHandler(void)
{
	DMA2D_IRQ_counter += 1;
	HAL_DMA2D_IRQHandler(&_hdma2d);
}
