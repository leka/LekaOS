/**
 * @file LekaScreen.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d);
void DMA2D_TransferErrorCallback(DMA2D_HandleTypeDef *hdma2d);
uint32_t JPEG_FindFrameOffset(uint32_t offset, FIL *file);
void OnError_Handler(const char *, int);

Screen::Screen()
	: _brightness(SCREEN_BACKLIGHT_PWM),
	  _interface(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK),
	  _file_interface("leka_fs"),
	  _sd_enable(SD_SPI_CS)
{
	_brightness.period(0.01f);	 // Set PWM at 1/(0.01 seconds) = 100Hz
	_brightness = 0.50f;
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
	// _sd_enable = 0;
	printf("Starting init... \n");
	int err = _interface.init();
	printf("Error value: %d\n", err);
	if (0 != err) {
		printf("Init failed \n");
		return -1;
	}
	printf("Init success \n");

	if (0 != _interface.frequency(5000000)) {
		printf("Error setting frequency \n");
	}
	printf("frequency set \n");

	return 0;
}

void Screen::getFileSize()
{
	static char filename[] = "image.jpg";
	FIL JPEG_File; /* File object */

	_file_interface.mount(&_interface);
	if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
		printf("File %s openened. File size : %lu \n\r", filename, f_size(&JPEG_File));

		f_close(&JPEG_File);
	}

	DIR *d;
	struct dirent *p;
	d = opendir("/leka_fs");
	if (d != NULL) {
		while ((p = readdir(d)) != NULL) {
			printf(" - %s\n", p->d_name);
		}
	} else {
		printf("Could not open directory!\n");
	}
	closedir(d);

	return;
}

// void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg);

void Screen::JPEGInit()
{
	JPEG_InitColorTables();

	_hjpeg.Instance = JPEG;
	// HAL_JPEG_MspInit(&_hjpeg);
	HAL_JPEG_Init(&_hjpeg);
}

void Screen::ScreenInit()
{
	// FROM BSP

	LCDReset();
	MSPInit();
	DSIInit();
	LTDCInit();
	OTM8009AInit();
}

void Screen::LCDReset()
{
	GPIO_InitTypeDef gpio_init_structure;

	__HAL_RCC_GPIOJ_CLK_ENABLE();

	/* Configure the GPIO on PJ15 */
	gpio_init_structure.Pin	  = GPIO_PIN_15;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_PULLUP;
	gpio_init_structure.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

	/* Activate XRES active low */
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_RESET);

	HAL_Delay(20); /* wait 20 ms */

	/* Desactivate XRES */
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_SET);

	/* Wait for 10ms after releasing XRES before sending commands */
	HAL_Delay(10);

	return;
}

void Screen::MSPInit()
{
	// MSP : MCU Support Package. https://stackoverflow.com/a/37520805

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

	return;
}

void Screen::DSIInit()
{
	DSI_PLLInitTypeDef dsiPllInit;
	uint32_t LcdClock = 27429; /*!< LcdClk = 27429 kHz */

	uint32_t laneByteClk_kHz = 0;
	// uint32_t VSA;  /*!< Vertical start active time in units of lines */
	// uint32_t VBP;  /*!< Vertical Back Porch time in units of lines */
	// uint32_t VFP;  /*!< Vertical Front Porch time in units of lines */
	// uint32_t VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
	// uint32_t HSA;  /*!< Horizontal start active time in units of lcdClk */
	// uint32_t HBP;  /*!< Horizontal Back Porch time in units of lcdClk */
	// uint32_t HFP;  /*!< Horizontal Front Porch time in units of lcdClk */
	// uint32_t HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */

	/* ADDED PARAMETER, NOT PRESENT IN BSP */
	// uint32_t lcd_x_size = OTM8009A_800X480_WIDTH;
	// uint32_t lcd_y_size = OTM8009A_800X480_HEIGHT;
	/* END OF ADDED PARAMETERS */

	/* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
	_hdsi.Instance = DSI;

	HAL_DSI_DeInit(&(_hdsi));

	dsiPllInit.PLLNDIV = 100;
	dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV5;
	dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;
	laneByteClk_kHz	   = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */

	/* Set number of Lanes */
	_hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

	/* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
	_hdsi.Init.TXEscapeCkdiv = laneByteClk_kHz / 15620;

	HAL_DSI_Init(&(_hdsi), &(dsiPllInit));

	/* Timing parameters for all Video modes
	 * Set Timing parameters of LTDC depending on its chosen orientation
	 */
	// if (orientation == LCD_ORIENTATION_PORTRAIT) {
	// 	lcd_x_size = OTM8009A_480X800_WIDTH;  /* 480 */
	// 	lcd_y_size = OTM8009A_480X800_HEIGHT; /* 800 */
	// } else {
	/* lcd_orientation == LCD_ORIENTATION_LANDSCAPE */
	// lcd_x_size = OTM8009A_800X480_WIDTH;  /* 800 */
	// lcd_y_size = OTM8009A_800X480_HEIGHT; /* 480 */
	// }

	// HACT = _screen_width;	 // Previously lcd_x_size, replace by _screen_width
	// VACT = _screen_height;	 // Previously lcd_y_size, replace by _screen_height

	/* The following values are same for portrait and landscape orientations */
	// VSA = OTM8009A_480X800_VSYNC; /* 12  */
	// VBP = OTM8009A_480X800_VBP;	  /* 12  */
	// VFP = OTM8009A_480X800_VFP;	  /* 12  */
	// HSA = OTM8009A_480X800_HSYNC; /* 63  */
	// HBP = OTM8009A_480X800_HBP;	  /* 120 */
	// HFP = OTM8009A_480X800_HFP;	  /* 120 */

	_hdsivideo.VirtualChannelID		= 0;			// Previously LCD_OTM8009A_ID = 0
	_hdsivideo.ColorCoding			= DSI_RGB888;	// Previously LCD_DSI_PIXEL_DATA_FMT_RBG888 = DSI_RGB888
	_hdsivideo.VSPolarity			= DSI_VSYNC_ACTIVE_HIGH;
	_hdsivideo.HSPolarity			= DSI_HSYNC_ACTIVE_HIGH;
	_hdsivideo.DEPolarity			= DSI_DATA_ENABLE_ACTIVE_HIGH;
	_hdsivideo.Mode					= DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
	_hdsivideo.NullPacketSize		= 0xFFF;
	_hdsivideo.NumberOfChunks		= 0;
	_hdsivideo.PacketSize			= HACT; /* Value depending on display orientation choice portrait/landscape */
	_hdsivideo.HorizontalSyncActive = (HSA * laneByteClk_kHz) / LcdClock;
	_hdsivideo.HorizontalBackPorch	= (HBP * laneByteClk_kHz) / LcdClock;
	_hdsivideo.HorizontalLine		= ((HACT + HSA + HBP + HFP) * laneByteClk_kHz) /
								LcdClock; /* Value depending on display orientation choice portrait/landscape */
	_hdsivideo.VerticalSyncActive = VSA;
	_hdsivideo.VerticalBackPorch  = VBP;
	_hdsivideo.VerticalFrontPorch = VFP;
	_hdsivideo.VerticalActive	  = VACT; /* Value depending on display orientation choice portrait/landscape */

	/* Enable or disable sending LP command while streaming is active in video mode */
	_hdsivideo.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */

	/* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
	/* Only useful when sending LP packets is allowed while streaming is active in video mode */
	_hdsivideo.LPLargestPacketSize = 16;

	/* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
	/* Only useful when sending LP packets is allowed while streaming is active in video mode */
	_hdsivideo.LPVACTLargestPacketSize = 0;

	/* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region
	 */
	/* while streaming is active in video mode */
	_hdsivideo.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;  /* Allow sending LP commands during HFP period */
	_hdsivideo.LPHorizontalBackPorchEnable	= DSI_LP_HBP_ENABLE;  /* Allow sending LP commands during HBP period */
	_hdsivideo.LPVerticalActiveEnable		= DSI_LP_VACT_ENABLE; /* Allow sending LP commands during VACT period */
	_hdsivideo.LPVerticalFrontPorchEnable	= DSI_LP_VFP_ENABLE;  /* Allow sending LP commands during VFP period */
	_hdsivideo.LPVerticalBackPorchEnable	= DSI_LP_VBP_ENABLE;  /* Allow sending LP commands during VBP period */
	_hdsivideo.LPVerticalSyncActiveEnable =
		DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */

	/* Configure DSI Video mode timings with settings set above */
	HAL_DSI_ConfigVideoMode(&(_hdsi), &(_hdsivideo));

	return;
}

void Screen::LTDCInit()
{
	/* Timing Configuration */
	_hltdc.Init.HorizontalSync	   = (HSA - 1);
	_hltdc.Init.AccumulatedHBP	   = (HSA + HBP - 1);
	_hltdc.Init.AccumulatedActiveW = (_screen_width + HSA + HBP - 1);
	_hltdc.Init.TotalWidth		   = (_screen_width + HSA + HBP + HFP - 1);

	/* Initialize the LCD pixel width and pixel height */
	_hltdc.LayerCfg->ImageWidth	 = _screen_width;
	_hltdc.LayerCfg->ImageHeight = _screen_height;

	/** LCD clock configuration
	 * Note: The following values should not be changed as the PLLSAI is also used
	 *      to clock the USB FS
	 * PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
	 * PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz
	 * PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.85 MHz
	 * LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.85 MHz / 2 = 27.429 MHz
	 */
	static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN		 = 384;
	PeriphClkInitStruct.PLLSAI.PLLSAIR		 = 7;
	PeriphClkInitStruct.PLLSAIDivR			 = RCC_PLLSAIDIVR_2;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	/* Background value */
	_hltdc.Init.Backcolor.Blue	= 0xFF;	  // Previously 0
	_hltdc.Init.Backcolor.Green = 0xFF;	  // Previously 0
	_hltdc.Init.Backcolor.Red	= 0xFF;	  // Previously 0
	_hltdc.Init.PCPolarity		= LTDC_PCPOLARITY_IPC;
	_hltdc.Instance				= LTDC;

	/* Get LTDC Configuration from DSI Configuration */
	HAL_LTDC_StructInitFromVideoConfig(&(_hltdc), &(_hdsivideo));

	/* Initialize the LTDC */
	HAL_LTDC_Init(&_hltdc);

	/* Enable the DSI host and wrapper after the LTDC initialization
	   To avoid any synchronization issue, the DSI shall be started after enabling the LTDC */
	HAL_DSI_Start(&_hdsi);

	// #if !defined(DATA_IN_ExtSDRAM)
	/* Initialize the SDRAM */
	// BSP_SDRAM_Init();
	SDRAMInit();
	// #endif /* DATA_IN_ExtSDRAM */

	/* Initialize the font */
	// BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
}

void Screen::SDRAMInit()
{
	FMC_SDRAM_TimingTypeDef Timing;	  // ADDED

	/* SDRAM device configuration */
	_hsdram.Instance = FMC_SDRAM_DEVICE;

	/* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
	Timing.LoadToActiveDelay	= 2;
	Timing.ExitSelfRefreshDelay = 7;
	Timing.SelfRefreshTime		= 4;
	Timing.RowCycleDelay		= 7;
	Timing.WriteRecoveryTime	= 2;
	Timing.RPDelay				= 2;
	Timing.RCDDelay				= 2;

	_hsdram.Init.SDBank				= FMC_SDRAM_BANK1;
	_hsdram.Init.ColumnBitsNumber	= FMC_SDRAM_COLUMN_BITS_NUM_8;
	_hsdram.Init.RowBitsNumber		= FMC_SDRAM_ROW_BITS_NUM_12;
	_hsdram.Init.MemoryDataWidth	= FMC_SDRAM_MEM_BUS_WIDTH_32;	// Previously SDRAM_MEMORY_WIDTH
	_hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	_hsdram.Init.CASLatency			= FMC_SDRAM_CAS_LATENCY_3;
	_hsdram.Init.WriteProtection	= FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	_hsdram.Init.SDClockPeriod		= FMC_SDRAM_CLOCK_PERIOD_2;	  // Previously SDCLOCK_PERIOD
	_hsdram.Init.ReadBurst			= FMC_SDRAM_RBURST_ENABLE;
	_hsdram.Init.ReadPipeDelay		= FMC_SDRAM_RPIPE_DELAY_0;

	/* SDRAM controller initialization */

	// BSP_SDRAM_MspInit(&sdramHandle, NULL); /* __weak function can be rewritten by the application */
	SDRAMMSPInit();

	uint8_t sdramstatus = 0x01;	  // Added
	if (HAL_SDRAM_Init(&_hsdram, &Timing) != HAL_OK) {
		sdramstatus = 0x01;
	} else {
		sdramstatus = 0x00;
	}

	/* SDRAM initialization sequence */
	// BSP_SDRAM_Initialization_sequence(REFRESH_COUNT);
	SDRAMInitSequence();

	return;
}

void Screen::SDRAMMSPInit()
{
	// static DMA_HandleTypeDef dma_handle;
	GPIO_InitTypeDef gpio_init_structure;

	/* Enable FMC clock */
	__HAL_RCC_FMC_CLK_ENABLE();

	/* Enable chosen DMAx clock */
	__HAL_RCC_DMA2_CLK_ENABLE();   // Previously: __DMAx_CLK_ENABLE();

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	/* Common GPIO configuration */
	gpio_init_structure.Mode	  = GPIO_MODE_AF_PP;
	gpio_init_structure.Pull	  = GPIO_PULLUP;
	gpio_init_structure.Speed	  = GPIO_SPEED_HIGH;
	gpio_init_structure.Alternate = GPIO_AF12_FMC;

	/* GPIOD configuration */
	gpio_init_structure.Pin =
		GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOD, &gpio_init_structure);

	/* GPIOE configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOE, &gpio_init_structure);

	/* GPIOF configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOF, &gpio_init_structure);

	/* GPIOG configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG, &gpio_init_structure);

	/* GPIOH configuration */
	gpio_init_structure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOH, &gpio_init_structure);

	/* GPIOI configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
							  GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOI, &gpio_init_structure);

	/* Configure common DMA parameters */
	_hdma.Init.Channel			   = DMA_CHANNEL_0;	  // Previsouly : SDRAM_DMAx_CHANNEL
	_hdma.Init.Direction		   = DMA_MEMORY_TO_MEMORY;
	_hdma.Init.PeriphInc		   = DMA_PINC_ENABLE;
	_hdma.Init.MemInc			   = DMA_MINC_ENABLE;
	_hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	_hdma.Init.MemDataAlignment	   = DMA_MDATAALIGN_WORD;
	_hdma.Init.Mode				   = DMA_NORMAL;
	_hdma.Init.Priority			   = DMA_PRIORITY_HIGH;
	_hdma.Init.FIFOMode			   = DMA_FIFOMODE_DISABLE;
	_hdma.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_FULL;
	_hdma.Init.MemBurst			   = DMA_MBURST_SINGLE;
	_hdma.Init.PeriphBurst		   = DMA_PBURST_SINGLE;

	_hdma.Instance = DMA2_Stream0;	 // Previously : SDRAM_DMAx_STREAM

	/* Associate the DMA handle */
	__HAL_LINKDMA(&_hsdram, hdma, _hdma);

	/* Deinitialize the stream for new transfer */
	HAL_DMA_DeInit(&_hdma);

	/* Configure the DMA stream */
	HAL_DMA_Init(&_hdma);

	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x0F, 0);	// Previously : SDRAM_DMAx_IRQn
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);				// Previously : SDRAM_DMAx_IRQn

	return;
}

void Screen::SDRAMInitSequence()
{
	uint32_t RefreshCount  = 0x0603;
	uint32_t SDRAM_TIMEOUT = 0xFFFF;

	__IO uint32_t tmpmrd = 0;

	/* Step 1: Configure a clock configuration enable command */
	_hsdramcmd.CommandMode			  = FMC_SDRAM_CMD_CLK_ENABLE;
	_hsdramcmd.CommandTarget		  = FMC_SDRAM_CMD_TARGET_BANK1;
	_hsdramcmd.AutoRefreshNumber	  = 1;
	_hsdramcmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&_hsdram, &_hsdramcmd, SDRAM_TIMEOUT);

	/* Step 2: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 3: Configure a PALL (precharge all) command */
	_hsdramcmd.CommandMode			  = FMC_SDRAM_CMD_PALL;
	_hsdramcmd.CommandTarget		  = FMC_SDRAM_CMD_TARGET_BANK1;
	_hsdramcmd.AutoRefreshNumber	  = 1;
	_hsdramcmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&_hsdram, &_hsdramcmd, SDRAM_TIMEOUT);

	/* Step 4: Configure an Auto Refresh command */
	_hsdramcmd.CommandMode			  = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	_hsdramcmd.CommandTarget		  = FMC_SDRAM_CMD_TARGET_BANK1;
	_hsdramcmd.AutoRefreshNumber	  = 8;
	_hsdramcmd.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&_hsdram, &_hsdramcmd, SDRAM_TIMEOUT);

	/* Step 5: Program the external memory mode register */
	// tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
	// 		 SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	tmpmrd = (uint32_t)0x0000 | 0x0000 | 0x0030 | 0x0000 | 0x0200;	 // by MODAR

	_hsdramcmd.CommandMode			  = FMC_SDRAM_CMD_LOAD_MODE;
	_hsdramcmd.CommandTarget		  = FMC_SDRAM_CMD_TARGET_BANK1;
	_hsdramcmd.AutoRefreshNumber	  = 1;
	_hsdramcmd.ModeRegisterDefinition = tmpmrd;

	/* Send the command */
	HAL_SDRAM_SendCommand(&_hsdram, &_hsdramcmd, SDRAM_TIMEOUT);

	/* Step 6: Set the refresh rate counter */
	/* Set the device refresh rate */
	HAL_SDRAM_ProgramRefreshRate(&_hsdram, RefreshCount);

	return;
}

void Screen::OTM8009AInit()
{
	/* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
	 *  depending on configuration set in 'hdsivideo_handle'.
	 */
	OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);
}

void Screen::DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
	if (NbrParams <= 1) {
		HAL_DSI_ShortWrite(&_hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
	} else {
		HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams);
	}
}

uint8_t Screen::OTM8009A_Init(uint32_t ColorCoding, uint32_t orientation)
{
	/* Enable CMD2 to access vendor specific commands                               */
	/* Enter in command 2 mode and set EXTC to enable address shift function (0x00) */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(3, (uint8_t *)lcdRegData1);

	/* Enter ORISE Command 2 */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData2); /* Shift address to 0x80 */
	DSI_IO_WriteCmd(2, (uint8_t *)lcdRegData2);

	/////////////////////////////////////////////////////////////////////
	/* SD_PCH_CTRL - 0xC480h - 129th parameter - Default 0x00          */
	/* Set SD_PT                                                       */
	/* -> Source output level during porch and non-display area to GND */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData2);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData3);
	HAL_Delay(10);
	/* Not documented */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData4);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData5);
	HAL_Delay(10);
	/////////////////////////////////////////////////////////////////////

	/* PWR_CTRL4 - 0xC4B0h - 178th parameter - Default 0xA8 */
	/* Set gvdd_en_test                                     */
	/* -> enable GVDD test mode !!!                         */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData6);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData7);

	/* PWR_CTRL2 - 0xC590h - 146th parameter - Default 0x79      */
	/* Set pump 4 vgh voltage                                    */
	/* -> from 15.0v down to 13.0v                               */
	/* Set pump 5 vgh voltage                                    */
	/* -> from -12.0v downto -9.0v                               */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData8);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData9);

	/* P_DRV_M - 0xC0B4h - 181th parameter - Default 0x00 */
	/* -> Column inversion                                */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData10);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData11);

	/* VCOMDC - 0xD900h - 1st parameter - Default 0x39h */
	/* VCOM Voltage settings                            */
	/* -> from -1.0000v downto -1.2625v                 */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData12);

	/* Oscillator adjustment for Idle/Normal mode (LPDT only) set to 65Hz (default is 60Hz) */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData13);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData14);

	/* Video mode internal */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData15);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData16);

	/* PWR_CTRL2 - 0xC590h - 147h parameter - Default 0x00 */
	/* Set pump 4&5 x6                                     */
	/* -> ONLY VALID when PUMP4_EN_ASDM_HV = "0"           */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData17);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData18);

	/* PWR_CTRL2 - 0xC590h - 150th parameter - Default 0x33h */
	/* Change pump4 clock ratio                              */
	/* -> from 1 line to 1/2 line                            */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData19);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData9);

	/* GVDD/NGVDD settings */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(2, (uint8_t *)lcdRegData5);

	/* PWR_CTRL2 - 0xC590h - 149th parameter - Default 0x33h */
	/* Rewrite the default value !                           */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData20);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData21);

	/* Panel display timing Setting 3 */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData22);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData23);

	/* Power control 1 */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData24);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData25);

	/* Source driver precharge */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData13);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData26);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData15);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData27);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData28);
	DSI_IO_WriteCmd(2, (uint8_t *)lcdRegData6);

	/* GOAVST */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData2);
	DSI_IO_WriteCmd(6, (uint8_t *)lcdRegData7);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData29);
	DSI_IO_WriteCmd(14, (uint8_t *)lcdRegData8);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData30);
	DSI_IO_WriteCmd(14, (uint8_t *)lcdRegData9);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData31);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData10);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData32);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData46);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData2);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData11);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData33);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData12);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData29);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData13);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData30);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData14);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData31);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData15);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData32);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData16);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData34);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData17);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData35);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData18);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData2);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData19);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData33);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData20);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData29);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData21);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData30);
	DSI_IO_WriteCmd(10, (uint8_t *)lcdRegData22);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData31);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData23);

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData32);
	DSI_IO_WriteCmd(15, (uint8_t *)lcdRegData24);

	/////////////////////////////////////////////////////////////////////////////
	/* PWR_CTRL1 - 0xc580h - 130th parameter - default 0x00 */
	/* Pump 1 min and max DM                                */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData13);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData47);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData48);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData49);
	/////////////////////////////////////////////////////////////////////////////

	/* CABC LEDPWM frequency adjusted to 19,5kHz */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData50);
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData51);

	/* Exit CMD2 mode */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(3, (uint8_t *)lcdRegData25);

	/*************************************************************************** */
	/* Standard DCS Initialization TO KEEP CAN BE DONE IN HSDT                   */
	/*************************************************************************** */

	/* NOP - goes back to DCS std command ? */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);

	/* Gamma correction 2.2+ table (HSDT possible) */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(16, (uint8_t *)lcdRegData3);

	/* Gamma correction 2.2- table (HSDT possible) */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);
	DSI_IO_WriteCmd(16, (uint8_t *)lcdRegData4);

	/* Send Sleep Out command to display : no parameter */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData36);

	/* Wait for sleep out exit */
	HAL_Delay(120);

	switch (ColorCoding) {
		case OTM8009A_FORMAT_RBG565:
			/* Set Pixel color format to RGB565 */
			DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData37);
			break;
		case OTM8009A_FORMAT_RGB888:
			/* Set Pixel color format to RGB888 */
			DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData38);
			break;
		default:
			break;
	}

	/* Send command to configure display in landscape orientation mode. By default
		the orientation mode is portrait  */
	if (orientation == OTM8009A_ORIENTATION_LANDSCAPE) {
		DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData39);
		DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData27);
		DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData28);
	}

	/** CABC : Content Adaptive Backlight Control section start >> */
	/* Note : defaut is 0 (lowest Brightness), 0xFF is highest Brightness, try 0x7F : intermediate value */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData40);

	/* defaut is 0, try 0x2C - Brightness Control Block, Display Dimming & BackLight on */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData41);

	/* defaut is 0, try 0x02 - image Content based Adaptive Brightness [Still Picture] */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData42);

	/* defaut is 0 (lowest Brightness), 0xFF is highest Brightness */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData43);

	/** CABC : Content Adaptive Backlight Control section end << */

	/* Send Command Display On */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData44);

	/* NOP command */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData1);

	/* Send Command GRAM memory write (no parameters) : this initiates frame write via other DSI commands sent by */
	/* DSI host from LTDC incoming pixels in video mode */
	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData45);

	return 0;
}

void Screen::LTDCLayerInit(uint16_t layer_index)
{
	LTDC_LayerCfgTypeDef Layercfg;

	/* Layer Init */
	Layercfg.WindowX0		 = 0;
	Layercfg.WindowX1		 = _screen_width;
	Layercfg.WindowY0		 = 0;
	Layercfg.WindowY1		 = _screen_height;
	Layercfg.PixelFormat	 = LTDC_PIXEL_FORMAT_ARGB8888;
	Layercfg.FBStartAdress	 = LCD_FRAME_BUFFER;   // Previously FB_Address given in parameter
	Layercfg.Alpha			 = 255;
	Layercfg.Alpha0			 = 0;
	Layercfg.Backcolor.Blue	 = 0;
	Layercfg.Backcolor.Green = 0;
	Layercfg.Backcolor.Red	 = 0;
	Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	Layercfg.ImageWidth		 = _screen_width;
	Layercfg.ImageHeight	 = _screen_height;

	HAL_LTDC_ConfigLayer(&_hltdc, &Layercfg, layer_index);

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
	fillBuffer(_active_layer, (uint32_t *)(_hltdc.LayerCfg[_active_layer].FBStartAdress), _screen_width, _screen_height,
			   0, ColorIndex);
}

void Screen::drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex)
{
	uint32_t Xaddress = 0;

	/* Set the text color */
	// BSP_LCD_SetTextColor(DrawProp[_active_layer].TextColor);

	/* Get the rectangle start address */
	Xaddress		= (_hltdc.LayerCfg[_active_layer].FBStartAdress) + 4 * (_screen_width * Ypos + Xpos);
	uint32_t offset = _screen_width - Width;

	/* Fill the rectangle */
	fillBuffer(_active_layer, (uint32_t *)Xaddress, Width, Height, offset, ColorIndex);
}

void Screen::drawImage(uint32_t data, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	uint32_t destination = LCD_FRAME_BUFFER + (x + y * _screen_width) * 4;
	_hdma2d.Instance	 = DMA2D;

	_hdma2d.Init.Mode		  = DMA2D_M2M_BLEND;
	_hdma2d.Init.ColorMode	  = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset = _screen_width - width;

	// Foreground
	_hdma2d.LayerCfg[1].AlphaMode	   = DMA2D_NO_MODIF_ALPHA;
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	_hdma2d.LayerCfg[1].InputOffset	   = 0;
	_hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA;

	// Background
	_hdma2d.LayerCfg[0].AlphaMode	   = DMA2D_NO_MODIF_ALPHA;
	_hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
	_hdma2d.LayerCfg[0].InputOffset	   = _screen_width - width;

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

// extern uint32_t Previous_FrameSize;
int DSI_IRQ_counter	  = 0;
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

		DSI_IRQ_counter = 0;
		printf("\n\r--------Programm starting--------\n\r");

		// uint32_t isfirstFrame, currentFrameRate;
		// auto startTime = HAL_GetTick();
		// auto endTime   = HAL_GetTick();
		// char message[16];

		/*##-1- JPEG Initialization ################################################*/
		/* Init The JPEG Look Up Tables used for YCbCr to RGB conversion   */
		// JPEG_InitColorTables();
		/* Init the HAL JPEG driver */
		// JPEG_Handle.Instance = JPEG;
		// HAL_JPEG_Init(&JPEG_Handle);

		// BSP_LCD_Init();
		// printf("LCD_Init done \n\r");

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
		HAL_DSI_ShortWrite(&(_hdsi), 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON, 0x00);
		// HAL_DSI_Refresh(&hdsi_discovery);

		// printf("DSI IRQ calls : %d \n\r \n\r", DSI_IRQ_counter);
		//##-3- Link the micro SD disk I/O driver ##################################
		// if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		//##-4- Register the file system object to the FatFs module ##############
		// if (f_mount(&SDFatFs, (TCHAR const *)SDPath, 0) == FR_OK) {
		_file_interface.mount(&_interface);
		//##-5- Open the JPG file with read access #############################
		if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
			printf("File %s openened. File size : %lu \n\r", filename, f_size(&JPEG_File));

			printf("Before Decoding\n");
			// JPEG_Decode_DMA(&_hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);
			JPEG_DecodePolling(&_hjpeg, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);
			printf("After decoding\n");

			HAL_JPEG_GetInfo(&_hjpeg, &_hjpeginfo);

			uint16_t xPos = (_screen_width - _hjpeginfo.ImageWidth) / 2;
			uint16_t yPos = (_screen_height - _hjpeginfo.ImageHeight) / 2;
			if (_hjpeginfo.ChromaSubsampling == JPEG_420_SUBSAMPLING) {
				if ((_hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (_hjpeginfo.ImageWidth % 16);
			}

			if (_hjpeginfo.ChromaSubsampling == JPEG_422_SUBSAMPLING) {
				if ((_hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (_hjpeginfo.ImageWidth % 16);
			}

			if (_hjpeginfo.ChromaSubsampling == JPEG_444_SUBSAMPLING) {
				if ((_hjpeginfo.ImageWidth % 8) != 0) width_offset = (_hjpeginfo.ImageWidth % 8);
			}

			DMA2D_CopyBuffer((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos, yPos,
							 _hjpeginfo.ImageWidth, _hjpeginfo.ImageHeight, width_offset);
			printf("End of CopyBuffer\n");
			ThisThread::sleep_for(1s);
			//##-10- Close the avi file ##########################################
			f_close(&JPEG_File);

		} else
			printf("Failed to open file %s \n\r", filename);
		// } else
		// 	printf("Mount failed \n\r");
		// } else
		// 	printf("FATFS link failed\n\r");
		// printf("Frame offset %lu \n\r", FrameOffset);
		// while (true) {
		// }
	} else {
		uint32_t bg_color = 0xffffffff;
		// initialize and select layer 0
		LTDCLayerInit(0);
		// lcd.LTDCLayerInit(1);
		setActiveLayer(0);
		// clear layer 0 in white
		clear(bg_color);

		drawImage((uint32_t)IMAGE_DATA_EYE, 100, 100, IMAGE_WIDTH_EYE, IMAGE_HEIGHT_EYE);
		drawImage((uint32_t)IMAGE_DATA_EYE, 500, 100, IMAGE_WIDTH_EYE, IMAGE_HEIGHT_EYE);
		drawImage((uint32_t)IMAGE_DATA_MOUTH, 340, 290, IMAGE_WIDTH_MOUTH, IMAGE_HEIGHT_MOUTH);
	}

	// ThisThread::sleep_for(30s);
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
	_hdma2d.Init.OutputOffset  = _screen_width - ImageWidth;
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
	// uint32_t x			 = (_screen_width - _hjpeginfo.ImageWidth) / 2;
	// uint32_t y			 = (_screen_height - _hjpeginfo.ImageHeight) / 2;
	// uint32_t destination = (uint32_t)pDst + (y * _screen_width + x) * 4;
	// // printf("DMA copy buffer \n\r");
	// /*while (pending_buffer != -1) {
	// }*/
	// HAL_DMA2D_Start(&_hdma2d, (uint32_t)pSrc, destination, ImageWidth, ImageHeight);
	// HAL_DMA2D_PollForTransfer(&_hdma2d, 100);

	uint32_t destination = (uint32_t)pDst + (y * _screen_width + x) * 4;
	uint32_t source		 = (uint32_t)pSrc;

	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	_hdma2d.Init.Mode		   = DMA2D_M2M_PFC;
	_hdma2d.Init.ColorMode	   = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset  = _screen_width - xsize;
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
	OnError_Handler(__FILE__, __LINE__);
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
	// getFileSize();

	// /* Enable I-Cache */
	// SCB_EnableICache();
	// /* Enable D-Cache */
	// SCB_EnableDCache();
	// printf("End of cache\n");

	// HAL_Init();
	// printf("End of HAL init\n");

	ScreenInit();
	JPEGInit();

	showFace(true);
	rtos::ThisThread::sleep_for(30s);

	while (true) {
		squareBouncing();
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}

/**
 * @brief  On Error Handler.
 * @param  None
 * @retval None
 */
void OnError_Handler(const char *file, int line)
{
	printf("Error crash in %s line %d\n\r", file, line);
	while (1) {
		;
	} /* Blocking on error */
}

void Screen::DSI_IRQHandler(void)
{
	DSI_IRQ_counter += 1;
	HAL_DSI_IRQHandler(&_hdsi);
}

void Screen::HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi)
{
	HAL_error_status = 1;
}

void Screen::DMA2D_IRQHandler(void)
{
	DMA2D_IRQ_counter += 1;
	HAL_DMA2D_IRQHandler(&_hdma2d);
}

void Screen::JPEG_IRQHandler(void)
{
	HAL_JPEG_IRQHandler(&_hjpeg);
}

void Screen::DMA2_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(_hjpeg.hdmain);
}

void Screen::DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(_hjpeg.hdmaout);
}

void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
	static DMA_HandleTypeDef hdmaIn;
	static DMA_HandleTypeDef hdmaOut;

	/* Enable JPEG clock */
	__HAL_RCC_JPEG_CLK_ENABLE();

	/* Enable DMA clock */
	__HAL_RCC_DMA2_CLK_ENABLE();

	HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
	HAL_NVIC_EnableIRQ(JPEG_IRQn);

	/* Input DMA */
	/* Set the parameters to be configured */
	hdmaIn.Init.Channel				= DMA_CHANNEL_9;
	hdmaIn.Init.Direction			= DMA_MEMORY_TO_PERIPH;
	hdmaIn.Init.PeriphInc			= DMA_PINC_DISABLE;
	hdmaIn.Init.MemInc				= DMA_MINC_ENABLE;
	hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaIn.Init.MemDataAlignment	= DMA_MDATAALIGN_WORD;
	hdmaIn.Init.Mode				= DMA_NORMAL;
	hdmaIn.Init.Priority			= DMA_PRIORITY_HIGH;
	hdmaIn.Init.FIFOMode			= DMA_FIFOMODE_ENABLE;
	hdmaIn.Init.FIFOThreshold		= DMA_FIFO_THRESHOLD_FULL;
	hdmaIn.Init.MemBurst			= DMA_MBURST_INC4;
	hdmaIn.Init.PeriphBurst			= DMA_PBURST_INC4;

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
	hdmaOut.Init.Channel			 = DMA_CHANNEL_9;
	hdmaOut.Init.Direction			 = DMA_PERIPH_TO_MEMORY;
	hdmaOut.Init.PeriphInc			 = DMA_PINC_DISABLE;
	hdmaOut.Init.MemInc				 = DMA_MINC_ENABLE;
	hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaOut.Init.MemDataAlignment	 = DMA_MDATAALIGN_WORD;
	hdmaOut.Init.Mode				 = DMA_NORMAL;
	hdmaOut.Init.Priority			 = DMA_PRIORITY_VERY_HIGH;
	hdmaOut.Init.FIFOMode			 = DMA_FIFOMODE_ENABLE;
	hdmaOut.Init.FIFOThreshold		 = DMA_FIFO_THRESHOLD_FULL;
	hdmaOut.Init.MemBurst			 = DMA_MBURST_INC4;
	hdmaOut.Init.PeriphBurst		 = DMA_PBURST_INC4;

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

void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef *hjpeg)
{
	HAL_NVIC_DisableIRQ(DMA2_Stream4_IRQn);

	/* DeInitialize the MDMA Stream */
	HAL_DMA_DeInit(hjpeg->hdmain);

	/* DeInitialize the MDMA Stream */
	HAL_DMA_DeInit(hjpeg->hdmaout);
}

#define VECT_TAB_OFFSET 0x00

void SystemInit(void)
{
	/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x24003010;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

#if defined(DATA_IN_ExtSDRAM)
	SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSDRAM */

	/* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
	SCB->VTOR = SRAM1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
	SCB->VTOR						   = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

#if defined(DATA_IN_ExtSDRAM)
static void SystemInit_ExtMemCtl(void);
#endif /* DATA_IN_ExtSDRAM */

#if defined(DATA_IN_ExtSDRAM)
/**
 * @brief  Setup the external memory controller.
 *         Called in startup_stm32f7xx.s before jump to main.
 *         This function configures the external memories (SDRAM)
 *         This SDRAM will be used as program data memory (including heap and stack).
 * @param  None
 * @retval None
 */
void SystemInit_ExtMemCtl(void)
{
	register uint32_t tmpreg = 0, timeout = 0xFFFF;
	register __IO uint32_t index;

	/* Enable GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface
	clock */
	RCC->AHB1ENR |= 0x000001F8;

	/* Connect PDx pins to FMC Alternate function */
	GPIOD->AFR[0] = 0x000000CC;
	GPIOD->AFR[1] = 0xCC000CCC;
	/* Configure PDx pins in Alternate function mode */
	GPIOD->MODER = 0xA02A000A;
	/* Configure PDx pins speed to 100 MHz */
	GPIOD->OSPEEDR = 0xF03F000F;
	/* Configure PDx pins Output type to push-pull */
	GPIOD->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PDx pins */
	GPIOD->PUPDR = 0x50150005;

	/* Connect PEx pins to FMC Alternate function */
	GPIOE->AFR[0] = 0xC00000CC;
	GPIOE->AFR[1] = 0xCCCCCCCC;
	/* Configure PEx pins in Alternate function mode */
	GPIOE->MODER = 0xAAAA800A;
	/* Configure PEx pins speed to 100 MHz */
	GPIOE->OSPEEDR = 0xFFFFC00F;
	/* Configure PEx pins Output type to push-pull */
	GPIOE->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PEx pins */
	GPIOE->PUPDR = 0x55554005;

	/* Connect PFx pins to FMC Alternate function */
	GPIOF->AFR[0] = 0x00CCCCCC;
	GPIOF->AFR[1] = 0xCCCCC000;
	/* Configure PFx pins in Alternate function mode */
	GPIOF->MODER = 0xAA800AAA;
	/* Configure PFx pins speed to 100 MHz */
	GPIOF->OSPEEDR = 0xFFC00FFF;
	/* Configure PFx pins Output type to push-pull */
	GPIOF->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PFx pins */
	GPIOF->PUPDR = 0x55400555;

	/* Connect PGx pins to FMC Alternate function */
	GPIOG->AFR[0] = 0x00CC0CCC;
	GPIOG->AFR[1] = 0xC000000C;
	/* Configure PGx pins in Alternate function mode */
	GPIOG->MODER = 0x80020A2A;
	/* Configure PGx pins speed to 100 MHz */
	GPIOG->OSPEEDR = 0xC0030F3F;
	/* Configure PGx pins Output type to push-pull */
	GPIOG->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PGx pins */
	GPIOG->PUPDR = 0x40010515;

	/* Connect PHx pins to FMC Alternate function */
	GPIOH->AFR[0] = 0x00C0CC00;
	GPIOH->AFR[1] = 0xCCCCCCCC;
	/* Configure PHx pins in Alternate function mode */
	GPIOH->MODER = 0xAAAA08A0;
	/* Configure PHx pins speed to 100 MHz */
	GPIOH->OSPEEDR = 0xFFFF0CF0;
	/* Configure PHx pins Output type to push-pull */
	GPIOH->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PHx pins */
	GPIOH->PUPDR = 0x55550450;

	/* Connect PIx pins to FMC Alternate function */
	GPIOI->AFR[0] = 0xCCCCCCCC;
	GPIOI->AFR[1] = 0x00000CC0;
	/* Configure PIx pins in Alternate function mode */
	GPIOI->MODER = 0x0028AAAA;
	/* Configure PIx pins speed to 100 MHz */
	GPIOI->OSPEEDR = 0x003CFFFF;
	/* Configure PIx pins Output type to push-pull */
	GPIOI->OTYPER = 0x00000000;
	/* No pull-up, pull-down for PIx pins */
	GPIOI->PUPDR = 0x00145555;

	/* Enable the FMC interface clock */
	RCC->AHB3ENR |= 0x00000001;

	/* Configure and enable SDRAM bank1 */
	FMC_Bank5_6->SDCR[0] = 0x000019E4;
	FMC_Bank5_6->SDTR[0] = 0x01116361;

	/* SDRAM initialization sequence */
	/* Clock enable command */
	FMC_Bank5_6->SDCMR = 0x00000011;
	tmpreg			   = FMC_Bank5_6->SDSR & 0x00000020;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Delay */
	for (index = 0; index < 1000; index++)
		;

	/* PALL command */
	FMC_Bank5_6->SDCMR = 0x00000012;
	timeout			   = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Auto refresh command */
	FMC_Bank5_6->SDCMR = 0x000000F3;
	timeout			   = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* MRD register program */
	FMC_Bank5_6->SDCMR = 0x00046014;
	timeout			   = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Set refresh count */
	tmpreg			   = FMC_Bank5_6->SDRTR;
	FMC_Bank5_6->SDRTR = (tmpreg | (0x00000603 << 1));

	/* Disable write protection */
	tmpreg				 = FMC_Bank5_6->SDCR[0];
	FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);

	/*
	 * Disable the FMC bank1 (enabled after reset).
	 * This, prevents CPU speculation access on this bank which blocks the use of FMC during
	 * 24us. During this time the others FMC master (such as LTDC) cannot use it!
	 */
	FMC_Bank1->BTCR[0] = 0x000030d2;
}
#endif /* DATA_IN_ExtSDRAM */