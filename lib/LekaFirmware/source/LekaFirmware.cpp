/**
 * @file LekaFirmware.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */

#if !DEVICE_QSPI
#error[NOT_SUPPORTED] QSPI not supported for this target
#endif

#include "LekaFirmware.h"

Firmware::Firmware()
	: _interface(QSPI_FLASH_IO0, QSPI_FLASH_IO1, QSPI_FLASH_IO2, QSPI_FLASH_IO3, QSPI_FLASH_CLK, QSPI_FLASH_nCS),
	  _disable_ext_memory_1(QSPI_FLASH_nCS_1, 1),
	  _disable_ext_memory_2(QSPI_FLASH_nCS_2, 1),
	  _disable_ext_memory_3(QSPI_FLASH_nCS_3, 1)
{
	ThisThread::sleep_for(10ms);
	init();
}

void Firmware::init()
{
	qspi_status_t qspi_status				 = QSPI_STATUS_OK;
	ext_flash_status_t external_flash_status = EXTERNAL_FLASH_STATUS_OK;

	/* Set QSPI interface configuration */
	qspi_status = _interface.configure_format(QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_ADDR_SIZE_24,
											  QSPI_CFG_BUS_SINGLE, QSPI_CFG_ALT_SIZE_8, QSPI_CFG_BUS_SINGLE, 0);
	if (qspi_status != QSPI_STATUS_OK) {
		printf("Config format failed\n");
		return;
	}

	/* Init 3 flash external memory */
	for (uint8_t memory_number = 1; memory_number < 4; memory_number++) {
		selectMemory(memory_number);
		external_flash_status = initExternalFlash();
	}

	if (external_flash_status != EXTERNAL_FLASH_STATUS_OK) {
		printf("Init failed\n");
		return;
	}

	return;
}

ext_flash_status_t Firmware::initExternalFlash()
{
	qspi_status_t status			   = QSPI_STATUS_OK;
	char status_value[STATUS_REG_SIZE] = {0};

	/* Read the Status Register from device */
	status = _interface.command_transfer(CMD_RDSR, -1, NULL, 0, status_value, STATUS_REG_SIZE);
	if (status != QSPI_STATUS_OK) {
		printf("Reading Status Register failed: value = 0x%x\n", (int)status_value[0]);
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	/* Send Reset Enable */
	status = _interface.command_transfer(CMD_RSTEN, -1, NULL, 0, NULL, 0);
	if (status != QSPI_STATUS_OK) {
		printf("Sending RSTEN failed \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (false == memoryIsReady()) {
		printf("Device not ready \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	/* Send Reset */
	status = _interface.command_transfer(CMD_RST, -1, NULL, 0, NULL, 0);
	if (status != QSPI_STATUS_OK) {
		printf("Sending RST failed \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (false == memoryIsReady()) {
		printf("Device not ready \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (status == QSPI_STATUS_OK) {
		return EXTERNAL_FLASH_STATUS_OK;
	} else {
		return EXTERNAL_FLASH_STATUS_ERROR;
	}
}

void Firmware::selectMemory(uint8_t memory_number)
{
	if (memory_number == _selected_memory) {
		return;
	}

	_disable_ext_memory_1 = 1;
	_disable_ext_memory_2 = 1;
	_disable_ext_memory_3 = 1;

	switch (memory_number) {
		case 1:
			_disable_ext_memory_1 = 0;
			_selected_memory	  = 1;
			break;
		case 2:
			_disable_ext_memory_2 = 0;
			_selected_memory	  = 2;
			break;
		case 3:
			_disable_ext_memory_3 = 0;
			_selected_memory	  = 3;
			break;
		default:
			_selected_memory = 0;
			break;
	}

	return;
}

bool Firmware::memoryIsReady()
{
	char status_value[STATUS_REG_SIZE] = {0xFF};
	int retries						   = 10000;
	bool mem_ready					   = true;

	do {
		retries--;
		if (QSPI_STATUS_OK != _interface.command_transfer(CMD_RDSR, -1, NULL, 0, status_value, STATUS_REG_SIZE)) {
			printf("Reading Status Register failed \n");
		}
		ThisThread::sleep_for(1ms);
	} while ((status_value[0] & BIT_WIP) != 0 && retries);

	if ((status_value[0] & BIT_WIP) != 0) {
		printf("mem_ready FALSE: status value = 0x%x\n", (int)status_value[0]);
		mem_ready = false;
	}
	return mem_ready;
}

ext_flash_status_t Firmware::writeEnable()
{
	char status_value[STATUS_REG_SIZE] = {0};
	ext_flash_status_t status		   = EXTERNAL_FLASH_STATUS_ERROR;

	if (QSPI_STATUS_OK != _interface.command_transfer(CMD_WREN, -1, NULL, 0, NULL, 0)) {
		printf("Sending WREN command FAILED \n");
		return status;
	}

	if (false == memoryIsReady()) {
		printf("Device not ready \n");
		return status;
	}

	if (QSPI_STATUS_OK != _interface.command_transfer(CMD_RDSR, -1, NULL, 0, status_value, STATUS_REG_SIZE)) {
		printf("Reading Status Register failed \n");
		return status;
	}

	if ((status_value[0] & BIT_WEL)) {
		status = EXTERNAL_FLASH_STATUS_OK;
	}
	return status;
}

ext_flash_status_t Firmware::eraseSector(uint8_t sector_address)
{
	if (0 != writeEnable()) {
		printf("Write Enable failed \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (QSPI_STATUS_OK !=
		_interface.command_transfer(CMD_ERASE, (((int)sector_address) & 0x00FFF000), NULL, 0, NULL, 0)) {
		printf("Erase failed\n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (false == memoryIsReady()) {
		printf("Device not ready \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	return EXTERNAL_FLASH_STATUS_OK;
}

ext_flash_status_t Firmware::write(uint8_t address, const char *buffer, size_t buffer_length, bool priorErase)
{
	qspi_status_t result;

	if (priorErase && eraseSector(address) != EXTERNAL_FLASH_STATUS_ERROR) {
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	if (0 != writeEnable()) {
		printf("Write Enable failed \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	result = _interface.write(CMD_WRITE, -1, address, buffer, &buffer_length);
	if (result != QSPI_STATUS_OK) {
		printf("Write failed\n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}
	// printf("Write done: %s \n", buffer);

	return EXTERNAL_FLASH_STATUS_OK;
}

ext_flash_status_t Firmware::read(uint8_t address, char *buffer, size_t buffer_length)
{
	qspi_status_t result;

	if (memoryIsReady() == false) {
		printf("Device not ready \n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	result = _interface.read(CMD_READ, -1, address, buffer, &buffer_length);
	if (result != QSPI_STATUS_OK) {
		printf("Read failed\n");
		return EXTERNAL_FLASH_STATUS_ERROR;
	}

	// printf("   %04X  ", address);
	// for (uint8_t j = 0; j < buffer_length; j++) { printf("%02X", buffer[j]); }
	// printf("\n");

	return EXTERNAL_FLASH_STATUS_OK;
}

void Firmware::checkActiveFlash()
{
	const uint16_t message_size			= 0x100;
	char message_to_write[message_size] = {0};
	char buffer[message_size]			= {0};
	for (uint16_t content = 0x00; content < message_size; content += 0x01) {
		message_to_write[content] = (uint8_t)content;
	}

	/* Write on flash */
	for (uint32_t address = 0x0; address < ExtFlashSize; address += message_size) {
		write(address, message_to_write, message_size, true);	// Erase before write
	}

	/* Read to check good writing*/
	for (uint32_t address = 0x0; address < ExtFlashSize; address += message_size) {
		for (uint16_t i = 0; i < message_size; i++) {
			buffer[i] = 0x00;
		}

		read(address, buffer, message_size);
		if (memcmp(message_to_write, buffer, message_size) != 0) {
			printf("Error on external memory n°%d at address %lX\n", _selected_memory, address);
			return;
		}
	}
	printf("External memory n°%d is OK!\n", _selected_memory);
	return;
}

void Firmware::start()
{
	printf("Firmware example\n\n");

	while (true) {
		for (uint8_t memoryNumber = 1; memoryNumber < 4; memoryNumber++) {
			selectMemory(memoryNumber);
			checkActiveFlash();
		}

		ThisThread::sleep_for(10s);
	}

	printf("End of Firmware example\n\n");
}
