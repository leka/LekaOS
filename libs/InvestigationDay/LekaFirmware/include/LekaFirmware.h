// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FIRMWARE_H_
#define _LEKA_OS_LIB_FIRMWARE_H_

#include <chrono>

#include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/QSPI.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "IS25LP016D.h"

typedef enum ext_flash_status
{
	EXTERNAL_FLASH_STATUS_ERROR				= 1, /**< Generic error >*/
	EXTERNAL_FLASH_STATUS_INVALID_PARAMETER = 2, /**< The parameter is invalid >*/
	EXTERNAL_FLASH_STATUS_OK				= 0, /**< Function executed sucessfully  >*/
} ext_flash_status_t;

class Firmware
{
  public:
	Firmware();
	~Firmware() {};

	void init();
	ext_flash_status_t initExternalFlash();

	void selectMemory(uint8_t memory_number);
	bool memoryIsReady();

	ext_flash_status writeEnable();
	ext_flash_status_t eraseSector(uint8_t address);
	ext_flash_status_t write(uint8_t address, const char *buffer, size_t buffer_length, bool priorErase = false);
	ext_flash_status_t read(uint8_t address, char *buffer, size_t buffer_length);

	void checkActiveFlash();
	void start(void);

  private:
	mbed::QSPI _interface;
	uint8_t _selected_memory;
	mbed::DigitalOut _disable_ext_memory_1;
	mbed::DigitalOut _disable_ext_memory_2;
	mbed::DigitalOut _disable_ext_memory_3;
};

#endif
