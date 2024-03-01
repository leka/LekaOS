// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define ATT_NUM_SIMUL_NTF										   1	// set by library:cordio
#define ATT_NUM_SIMUL_WRITE_CMD									   1	// set by library:cordio
#define BLE_FEATURE_EXTENDED_ADVERTISING						   1	// set by library:ble
#define BLE_FEATURE_GATT_CLIENT									   1	// set by library:ble
#define BLE_FEATURE_GATT_SERVER									   1	// set by library:ble
#define BLE_FEATURE_PERIODIC_ADVERTISING						   1	// set by library:ble
#define BLE_FEATURE_PHY_MANAGEMENT								   1	// set by library:ble
#define BLE_FEATURE_PRIVACY										   1	// set by library:ble
#define BLE_FEATURE_SECURE_CONNECTIONS							   1	// set by library:ble
#define BLE_FEATURE_SECURITY									   1	// set by library:ble
#define BLE_FEATURE_SIGNING										   1	// set by library:ble
#define BLE_FEATURE_WHITELIST									   1	// set by library:ble
#define BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION			   1	// set by library:ble
#define BLE_GAP_HOST_PRIVACY_RESOLVED_CACHE_SIZE				   16	// set by library:ble
#define BLE_GAP_MAX_ADVERTISING_REPORTS_PENDING_ADDRESS_RESOLUTION 16	// set by library:ble
#define BLE_GAP_MAX_ADVERTISING_SETS							   15	// set by library:ble
#define BLE_ROLE_BROADCASTER									   1	// set by library:ble
#define BLE_ROLE_CENTRAL										   1	// set by library:ble
#define BLE_ROLE_OBSERVER										   1	// set by library:ble
#define BLE_ROLE_PERIPHERAL										   1	// set by library:ble
#define BLE_SECURITY_DATABASE_FILESYSTEM						   1	// set by library:ble
#define BLE_SECURITY_DATABASE_KVSTORE							   0	// set by library:ble
#define BLE_SECURITY_DATABASE_MAX_ENTRIES						   5	// set by library:ble
#define CLOCK_SOURCE											   USE_PLL_HSE_EXTC | USE_PLL_HSI	// set by target:MCU_STM32F7
#define DM_CONN_MAX												   3					   // set by library:cordio
#define DM_NUM_ADV_SETS											   3					   // set by library:cordio
#define DM_NUM_PHYS												   3					   // set by library:cordio
#define DM_SYNC_MAX												   1					   // set by library:cordio
#define EATT_CONN_CHAN_MAX										   1					   // set by library:cordio
#define L2C_COC_CHAN_MAX										   1					   // set by library:cordio
#define L2C_COC_REG_MAX											   1					   // set by library:cordio
#define LPTICKER_DELAY_TICKS									   0					   // set by target:MCU_STM32F7
#define MBED_CONF_APP_TARGET_NAME								   "LEKA_Vx_x_UNIT_TEST"   // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLE_API_IMPLEMENTATION_MAX_CCCD_COUNT			   20	// set by library:ble-api-implementation
#define MBED_CONF_BLE_API_IMPLEMENTATION_MAX_CHARACTERISTIC_AUTHORISATION_COUNT                                        \
	20																	   // set by library:ble-api-implementation
#define MBED_CONF_BLE_PRESENT									 1		   // set by library:ble
#define MBED_CONF_BLUENRG_MS_SPI_IRQ							 BLE_IRQ   // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_SPI_MISO							 BLE_SPI_MISO	// set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_SPI_MOSI							 BLE_SPI_MOSI	// set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_SPI_NCS							 BLE_SPI_NSS   // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_SPI_RESET							 BLE_RESET	 // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_SPI_SCK							 BLE_SPI_SCK   // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_BLUENRG_MS_VALID_PUBLIC_BD_ADDRESS			 0			   // set by library:bluenrg_ms
#define MBED_CONF_CELLULAR_CONTROL_PLANE_OPT					 0			   // set by library:cellular
#define MBED_CONF_CELLULAR_DEBUG_AT								 0			   // set by library:cellular
#define MBED_CONF_CELLULAR_MAX_CP_DATA_RECV_LEN					 1358		   // set by library:cellular
#define MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY				 0			   // set by library:cellular
#define MBED_CONF_CELLULAR_USE_APN_LOOKUP						 0			   // set by library:cellular
#define MBED_CONF_CELLULAR_USE_SMS								 0			   // set by library:cellular
#define MBED_CONF_CORDIO_DESIRED_ATT_MTU						 23			   // set by library:cordio
#define MBED_CONF_CORDIO_MAX_PREPARED_WRITES					 4			   // set by library:cordio
#define MBED_CONF_CORDIO_PREFERRED_TX_POWER						 0			   // set by library:cordio
#define MBED_CONF_CORDIO_ROUTE_UNHANDLED_COMMAND_COMPLETE_EVENTS 1			   // set by library:cordio
#define MBED_CONF_CORDIO_RX_ACL_BUFFER_SIZE						 70			   // set by library:cordio
#define MBED_CONF_DRIVERS_OSPI_CSN								 OSPI_FLASH1_CSN   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_DQS								 OSPI_FLASH1_DQS   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO0								 OSPI_FLASH1_IO0   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO1								 OSPI_FLASH1_IO1   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO2								 OSPI_FLASH1_IO2   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO3								 OSPI_FLASH1_IO3   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO4								 OSPI_FLASH1_IO4   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO5								 OSPI_FLASH1_IO5   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO6								 OSPI_FLASH1_IO6   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_IO7								 OSPI_FLASH1_IO7   // set by library:drivers
#define MBED_CONF_DRIVERS_OSPI_SCK								 OSPI_FLASH1_SCK   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_CSN								 QSPI_FLASH1_CSN   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO0								 QSPI_FLASH1_IO0   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO1								 QSPI_FLASH1_IO1   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO2								 QSPI_FLASH1_IO2   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO3								 QSPI_FLASH1_IO3   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_SCK								 QSPI_FLASH1_SCK   // set by library:drivers
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE				 256			   // set by library:drivers
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE				 256			   // set by library:drivers
#define MBED_CONF_ESP8266_BUILT_IN_DNS							 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_DEBUG									 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_OFF_TIME_MS						 3				   // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_ON_POLARITY						 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_ON_TIME_MS						 3				   // set by library:esp8266
#define MBED_CONF_ESP8266_PROVIDE_DEFAULT						 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_SERIAL_BAUDRATE						 115200			   // set by library:esp8266
#define MBED_CONF_ESP8266_SNTP_ENABLE							 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_SNTP_SERVER0							 ""				   // set by library:esp8266
#define MBED_CONF_ESP8266_SNTP_SERVER1							 ""				   // set by library:esp8266
#define MBED_CONF_ESP8266_SNTP_SERVER2							 ""				   // set by library:esp8266
#define MBED_CONF_ESP8266_SNTP_TIMEZONE							 0				   // set by library:esp8266
#define MBED_CONF_ESP8266_SOCKET_BUFSIZE						 8192			   // set by library:esp8266
#define MBED_CONF_EVENTS_PRESENT								 1				   // set by library:events
#define MBED_CONF_EVENTS_SHARED_DISPATCH_FROM_APPLICATION		 0				   // set by library:events
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE						 768			   // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE				 256			   // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE				 1024			   // set by library:events
#define MBED_CONF_EVENTS_SHARED_STACKSIZE						 2048			   // set by library:events
#define MBED_CONF_EVENTS_USE_LOWPOWER_TIMER_TICKER				 0				   // set by library:events
#define MBED_CONF_FAT_CHAN_FFS_DBG								 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_CODE_PAGE							 437			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_EXFAT							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_HEAPBUF						 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_LOCK							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_MINIMIZE						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_NOFSINFO						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_NORTC							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_READONLY						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_REENTRANT						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_RPATH							 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_TIMEOUT						 1000			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_TINY							 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_LFN_BUF							 255			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_LFN_UNICODE						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MAX_LFN							 255			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MAX_SS							 4096			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MIN_SS							 512			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MULTI_PARTITION					 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_MDAY						 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_MON							 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_YEAR						 2017			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_SFN_BUF							 12				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_STRF_ENCODE						 3				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_STR_VOLUME_ID						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_SYNC_T							 HANDLE			   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_CHMOD							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_EXPAND						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FASTSEEK						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FIND							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FORWARD						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_LABEL							 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_LFN							 3				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_MKFS							 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_STRFUNC						 0				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_TRIM							 1				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_VOLUMES							 4				   // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_VOLUME_STRS                                                                              \
	"RAM", "NAND", "CF", "SD", "SD2", "USB", "USB2", "USB3"		  // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FLUSH_ON_NEW_CLUSTER		 0			  // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FLUSH_ON_NEW_SECTOR		 1			  // set by library:fat_chan
#define MBED_CONF_FILESYSTEM_PRESENT				 1			  // set by library:filesystem
#define MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS 0xFFFFFFFF	  // set by library:flashiap-block-device
#define MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE		 0			  // set by library:flashiap-block-device
#define MBED_CONF_LORA_ADR_ON						 1			  // set by library:lora
#define MBED_CONF_LORA_APPLICATION_EUI                                                                                 \
	{                                                                                                                  \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
	}	// set by library:lora
#define MBED_CONF_LORA_APPLICATION_KEY                                                                                 \
	{                                                                                                                  \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                 \
	}	// set by library:lora
#define MBED_CONF_LORA_APPSKEY                                                                                         \
	{                                                                                                                  \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                 \
	}														 // set by library:lora
#define MBED_CONF_LORA_APP_PORT					15			 // set by library:lora
#define MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE 1			 // set by library:lora
#define MBED_CONF_LORA_DEVICE_ADDRESS			0x00000000	 // set by library:lora
#define MBED_CONF_LORA_DEVICE_EUI                                                                                      \
	{                                                                                                                  \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
	}												// set by library:lora
#define MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH 5	// set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON			1	// set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON_JOIN		1	// set by library:lora
#define MBED_CONF_LORA_FSB_MASK                                                                                        \
	{                                                                                                                  \
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF                                                                         \
	}	// set by library:lora
#define MBED_CONF_LORA_FSB_MASK_CHINA                                                                                  \
	{                                                                                                                  \
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF                                                                 \
	}										 // set by library:lora
#define MBED_CONF_LORA_LBT_ON			0	 // set by library:lora
#define MBED_CONF_LORA_MAX_SYS_RX_ERROR 5	 // set by library:lora
#define MBED_CONF_LORA_NB_TRIALS		12	 // set by library:lora
#define MBED_CONF_LORA_NWKSKEY                                                                                         \
	{                                                                                                                  \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                 \
	}																			  // set by library:lora
#define MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION							  1		  // set by library:lora
#define MBED_CONF_LORA_PHY												  EU868	  // set by library:lora
#define MBED_CONF_LORA_PUBLIC_NETWORK									  1		  // set by library:lora
#define MBED_CONF_LORA_TX_MAX_SIZE										  64	  // set by library:lora
#define MBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH							  8		  // set by library:lora
#define MBED_CONF_LORA_WAKEUP_TIME										  5		  // set by library:lora
#define MBED_CONF_NANOSTACK_LIBSERVICE_PRESENT							  1		  // set by library:nanostack-libservice
#define MBED_CONF_NSAPI_DEFAULT_MESH_TYPE								  THREAD   // set by library:nsapi
#define MBED_CONF_NSAPI_DEFAULT_STACK									  LWIP	   // set by library:nsapi
#define MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY							  NONE	   // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_ADDRESSES_LIMIT								  10	   // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_CACHE_SIZE									  3		   // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME							  10000	   // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_RETRIES										  1		   // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS								  10	   // set by library:nsapi
#define MBED_CONF_NSAPI_PRESENT											  1		   // set by library:nsapi
#define MBED_CONF_NSAPI_SOCKET_STATS_ENABLED							  0		   // set by library:nsapi
#define MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT							  10	   // set by library:nsapi
#define MBED_CONF_PLATFORM_CALLBACK_COMPARABLE							  1		   // set by library:platform
#define MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL							  0		   // set by library:platform
#define MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED						  0		   // set by library:platform
#define MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX								  8		   // set by library:platform
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE						  9600	   // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO						  0		   // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED				  0		   // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_ENABLED							  0		   // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE								  4		   // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_REBOOT_MAX								  1		   // set by library:platform
#define MBED_CONF_PLATFORM_FATAL_ERROR_AUTO_REBOOT_ENABLED				  0		   // set by library:platform
#define MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN						  16	   // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT					  1		   // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT			  0		   // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS 6		   // set by library:platform
#define MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER						  0		   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE								  9600	   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL						  0		   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES						  1		   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES					  1		   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT							  1		   // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY					  0		   // set by library:platform
#define MBED_CONF_PLATFORM_USE_MPU										  1		   // set by library:platform
#define MBED_CONF_RTOS_API_PRESENT										  1		   // set by library:rtos-api
#define MBED_CONF_RTOS_EVFLAGS_NUM										  0		   // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE							  512	   // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_DEBUG_EXTRA				  128	   // set by library:rtos[STM]
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_TICKLESS_EXTRA			  256	   // set by library:rtos
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE							  4096	   // set by library:rtos
#define MBED_CONF_RTOS_MSGQUEUE_DATA_SIZE								  0		   // set by library:rtos
#define MBED_CONF_RTOS_MSGQUEUE_NUM										  0		   // set by library:rtos
#define MBED_CONF_RTOS_MUTEX_NUM										  0		   // set by library:rtos
#define MBED_CONF_RTOS_PRESENT											  1		   // set by library:rtos
#define MBED_CONF_RTOS_SEMAPHORE_NUM									  0		   // set by library:rtos
#define MBED_CONF_RTOS_THREAD_NUM										  0		   // set by library:rtos
#define MBED_CONF_RTOS_THREAD_STACK_SIZE								  4096	   // set by library:rtos
#define MBED_CONF_RTOS_THREAD_USER_STACK_SIZE							  0		   // set by library:rtos
#define MBED_CONF_RTOS_TIMER_NUM										  0		   // set by library:rtos
#define MBED_CONF_RTOS_TIMER_THREAD_STACK_SIZE							  768	   // set by library:rtos
#define MBED_CONF_SD_CMD0_IDLE_STATE_RETRIES							  5		   // set by library:sd
#define MBED_CONF_SD_CMD_TIMEOUT										  10000	   // set by library:sd
#define MBED_CONF_SD_CRC_ENABLED										  0		   // set by library:sd
#define MBED_CONF_SD_FSFAT_SDCARD_INSTALLED								  1		   // set by library:sd
#define MBED_CONF_SD_INIT_FREQUENCY										  100000   // set by library:sd
#define MBED_CONF_SD_SPI_CLK											  SD_SPI_SCK   // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_SD_SPI_CS												  SD_SPI_CS	  // set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_SD_SPI_MISO											  SD_SPI_MISO	// set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_SD_SPI_MOSI											  SD_SPI_MOSI	// set by application[LEKA_Vx_x_UNIT_TEST]
#define MBED_CONF_SD_TEST_BUFFER										  8192		// set by library:sd
#define MBED_CONF_SD_TRX_FREQUENCY										  1000000	// set by library:sd
#define MBED_CONF_TARGET_BOOT_STACK_SIZE								  0x400		// set by library:rtos[*]
#define MBED_CONF_TARGET_CONSOLE_UART									  1			// set by target:Target
#define MBED_CONF_TARGET_DEEP_SLEEP_LATENCY								  4			// set by target:MCU_STM32
#define MBED_CONF_TARGET_DEFAULT_ADC_VREF								  NAN		// set by target:Target
#define MBED_CONF_TARGET_FLASH_DUAL_BANK								  0			// set by target:LEKA_Vx_x_UNIT_TEST
#define MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT							  1			// set by target:MCU_STM32
#define MBED_CONF_TARGET_INTERNAL_FLASH_UNIFORM_SECTORS					  0			// set by target:MCU_STM32F7
#define MBED_CONF_TARGET_LPTICKER_LPTIM									  1			// set by target:MCU_STM32F7
#define MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK							  1			// set by target:MCU_STM32
#define MBED_CONF_TARGET_LPUART_CLOCK_SOURCE							  USE_LPUART_CLK_LSE | USE_LPUART_CLK_PCLK1	  // set by target:MCU_STM32
#define MBED_CONF_TARGET_LSE_AVAILABLE									  0	  // set by target:LEKA_Vx_x_UNIT_TEST
#define MBED_CONF_TARGET_MPU_ROM_END									  0x0fffffff   // set by target:Target
#define MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE					  WIFI	   // set by target:LEKA_Vx_x_UNIT_TEST
#define MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER						  0		   // set by target:Target
#define MBED_CONF_TARGET_XIP_ENABLE										  0		   // set by target:Target
#define MBED_CRC_TABLE_SIZE												  16	   // set by library:drivers
#define MBED_LFS2_BLOCK_CYCLES											  1024	   // set by library:littlefs2
#define MBED_LFS2_BLOCK_SIZE											  512	   // set by library:littlefs2
#define MBED_LFS2_CACHE_SIZE											  64	   // set by library:littlefs2
#define MBED_LFS2_ENABLE_INFO											  0		   // set by library:littlefs2
#define MBED_LFS2_INTRINSICS											  1		   // set by library:littlefs2
#define MBED_LFS2_LOOKAHEAD_SIZE										  64	   // set by library:littlefs2
#define MBED_LFS_BLOCK_SIZE												  512	   // set by library:littlefs
#define MBED_LFS_ENABLE_INFO											  0		   // set by library:littlefs
#define MBED_LFS_INTRINSICS												  1		   // set by library:littlefs
#define MBED_LFS_LOOKAHEAD												  512	   // set by library:littlefs
#define MBED_LFS_PROG_SIZE												  64	   // set by library:littlefs
#define MBED_LFS_READ_SIZE												  64	   // set by library:littlefs
#define MBED_STACK_DUMP_ENABLED											  0		   // set by library:platform
#define MEM_ALLOC														  malloc   // set by library:mbed-trace
#define MEM_FREE														  free	   // set by library:mbed-trace
#define SEC_CCM_CFG														  1		   // set by library:cordio
#define SMP_DB_MAX_DEVICES												  3		   // set by library:cordio
#define TARGET_LSE_DRIVE_LOAD_LEVEL										  RCC_LSEDRIVE_LOW	 // set by target:MCU_STM32F7
#define USE_HAL_JPEG_REGISTER_CALLBACKS									  1U				 // set by application
#define USE_HAL_TIM_REGISTER_CALLBACKS									  1U				 // set by application
#define USE_HAL_DAC_REGISTER_CALLBACKS									  1U				 // set by application
// Macros
#define WSF_MS_PER_TICK 1	// defined by library:cordio
#define _RTE_				// defined by library:rtos

#define MBED_CPU_STATS_ENABLED	 1
#define MBED_HEAP_STATS_ENABLED	 1
#define MBED_STACK_STATS_ENABLED 1

#endif
