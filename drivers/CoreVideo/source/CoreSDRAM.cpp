#include "CoreSDRAM.hpp"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

CoreSDRAM::CoreSDRAM(interface::STM32Hal &hal) : _hal(hal)
{
	_hsdram.Instance = FMC_SDRAM_DEVICE;

	setupSDRAMConfig();
}

void CoreSDRAM::setupSDRAMConfig()
{
	_hsdram.Init.SDBank				= FMC_SDRAM_BANK1;
	_hsdram.Init.ColumnBitsNumber	= FMC_SDRAM_COLUMN_BITS_NUM_8;
	_hsdram.Init.RowBitsNumber		= FMC_SDRAM_ROW_BITS_NUM_12;
	_hsdram.Init.MemoryDataWidth	= sdram::memory_width;
	_hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	_hsdram.Init.CASLatency			= FMC_SDRAM_CAS_LATENCY_3;
	_hsdram.Init.WriteProtection	= FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	_hsdram.Init.SDClockPeriod		= sdram::sd_clock_period;
	_hsdram.Init.ReadBurst			= FMC_SDRAM_RBURST_ENABLE;
	_hsdram.Init.ReadPipeDelay		= FMC_SDRAM_RPIPE_DELAY_0;
}

auto CoreSDRAM::setupTimingConfig() -> FMC_SDRAM_TimingTypeDef
{
	FMC_SDRAM_TimingTypeDef timing;

	// Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz)
	timing.LoadToActiveDelay	= 2;
	timing.ExitSelfRefreshDelay = 7;
	timing.SelfRefreshTime		= 4;
	timing.RowCycleDelay		= 7;
	timing.WriteRecoveryTime	= 2;
	timing.RPDelay				= 2;
	timing.RCDDelay				= 2;

	return timing;
}

auto CoreSDRAM::setupDMA() -> DMA_HandleTypeDef
{
	DMA_HandleTypeDef hdma;

	hdma.Init.Channel			  = DMA_CHANNEL_0;
	hdma.Init.Direction			  = DMA_MEMORY_TO_MEMORY;
	hdma.Init.PeriphInc			  = DMA_PINC_ENABLE;
	hdma.Init.MemInc			  = DMA_MINC_ENABLE;
	hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma.Init.MemDataAlignment	  = DMA_MDATAALIGN_WORD;
	hdma.Init.Mode				  = DMA_NORMAL;
	hdma.Init.Priority			  = DMA_PRIORITY_HIGH;
	hdma.Init.FIFOMode			  = DMA_FIFOMODE_DISABLE;
	hdma.Init.FIFOThreshold		  = DMA_FIFO_THRESHOLD_FULL;
	hdma.Init.MemBurst			  = DMA_MBURST_SINGLE;
	hdma.Init.PeriphBurst		  = DMA_PBURST_SINGLE;

	hdma.Instance = DMA2_Stream0;

	return hdma;
}

auto CoreSDRAM::initialize() -> uint8_t
{
	static uint8_t sdram_status = sdram::status::error;

	FMC_SDRAM_TimingTypeDef timing = setupTimingConfig();

	// SDRAM controller initialization
	initializeController();

	if (_hal.HAL_SDRAM_Init(&_hsdram, &timing) != HAL_OK) {
		sdram_status = sdram::status::error;
	} else {
		sdram_status = sdram::status::ok;
	}

	// SDRAM initialization sequence
	initializationSequence();

	return sdram_status;
}

void CoreSDRAM::initializeController()
{
	GPIO_InitTypeDef gpio_init_structure;

	// Enable FMC clock
	_hal.HAL_RCC_FMC_CLK_ENABLE();

	// Enable chosen DMAx clock
	_hal.HAL_RCC_DMA2_CLK_ENABLE();

	// Enable GPIOs clock
	_hal.HAL_RCC_GPIOD_CLK_ENABLE();
	_hal.HAL_RCC_GPIOE_CLK_ENABLE();
	_hal.HAL_RCC_GPIOF_CLK_ENABLE();
	_hal.HAL_RCC_GPIOG_CLK_ENABLE();
	_hal.HAL_RCC_GPIOH_CLK_ENABLE();
	_hal.HAL_RCC_GPIOI_CLK_ENABLE();

	// Common GPIO configuration
	gpio_init_structure.Mode	  = GPIO_MODE_AF_PP;
	gpio_init_structure.Pull	  = GPIO_PULLUP;
	gpio_init_structure.Speed	  = GPIO_SPEED_HIGH;
	gpio_init_structure.Alternate = GPIO_AF12_FMC;

	// GPIOD configuration
	gpio_init_structure.Pin =
		GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;

	_hal.HAL_GPIO_Init(GPIOD, &gpio_init_structure);

	// GPIOE configuration
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	_hal.HAL_GPIO_Init(GPIOE, &gpio_init_structure);

	// GPIOF configuration
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	_hal.HAL_GPIO_Init(GPIOF, &gpio_init_structure);

	// GPIOG configuration
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	_hal.HAL_GPIO_Init(GPIOG, &gpio_init_structure);

	// GPIOH configuration
	gpio_init_structure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
							  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	_hal.HAL_GPIO_Init(GPIOH, &gpio_init_structure);

	// GPIOI configuration
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
							  GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
	_hal.HAL_GPIO_Init(GPIOI, &gpio_init_structure);

	// Configure common DMA parameters

	DMA_HandleTypeDef dma_handle = setupDMA();

	// Associate the DMA handle
	_hal.HAL_LINKDMA(&_hsdram, _hsdram.hdma, dma_handle);

	// Deinitialize the stream for new transfer
	_hal.HAL_DMA_DeInit(&dma_handle);

	// Configure the DMA stream
	_hal.HAL_DMA_Init(&dma_handle);

	// NVIC configuration for DMA transfer complete interrupt
	_hal.HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x0F, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void CoreSDRAM::initializationSequence()
{
	FMC_SDRAM_CommandTypeDef command;

	// Step 1: Configure a clock configuration enable command
	command.CommandMode			   = FMC_SDRAM_CMD_CLK_ENABLE;
	command.CommandTarget		   = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber	   = 1;
	command.ModeRegisterDefinition = 0;

	// Send the command
	_hal.HAL_SDRAM_SendCommand(&_hsdram, &command, sdram::timeout);

	// Step 2: Insert 100 us minimum delay
	// Inserted delay is equal to 1 ms due to systick time base unit (ms)
	rtos::ThisThread::sleep_for(1ms);

	// Step 3: Configure a PALL (precharge all) command
	command.CommandMode			   = FMC_SDRAM_CMD_PALL;
	command.CommandTarget		   = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber	   = 1;
	command.ModeRegisterDefinition = 0;

	// Send the command
	_hal.HAL_SDRAM_SendCommand(&_hsdram, &command, sdram::timeout);

	// Step 4: Configure an Auto Refresh command
	command.CommandMode			   = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	command.CommandTarget		   = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber	   = 8;
	command.ModeRegisterDefinition = 0;

	// Send the command
	_hal.HAL_SDRAM_SendCommand(&_hsdram, &command, sdram::timeout);

	// Step 5: Program the external memory mode register
	constexpr auto mode_register_definition =
		uint32_t {sdram::mode::burst::length1 | sdram::mode::burst::type::sequential | sdram::mode::cas::latency3 |
				  sdram::mode::operating_mode::standard | sdram::mode::writeburst_mode::single};

	command.CommandMode			   = FMC_SDRAM_CMD_LOAD_MODE;
	command.CommandTarget		   = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber	   = 1;
	command.ModeRegisterDefinition = mode_register_definition;

	// Send the command
	_hal.HAL_SDRAM_SendCommand(&_hsdram, &command, sdram::timeout);

	// Step 6: Set the refresh rate counter
	// Set the device refresh rate
	_hal.HAL_SDRAM_ProgramRefreshRate(&_hsdram, sdram::refresh_count);
}

auto CoreSDRAM::getHandle() const -> SDRAM_HandleTypeDef
{
	return _hsdram;
}
