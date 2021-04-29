/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define ARM_UC_HTTP_RESUME_EXPONENTIATION_FACTOR                              2                                                                                                 // set by library:update-client
#define ARM_UC_HTTP_RESUME_INITIAL_DELAY_SECS                                 5                                                                                                 // set by library:update-client
#define ARM_UC_HTTP_RESUME_MAXIMUM_DELAY_SECS                                 (60*60)                                                                                           // set by library:update-client
#define ARM_UC_HTTP_RESUME_MAXIMUM_DOWNLOAD_TIME_SECS                         (7*24*60*60)                                                                                      // set by library:update-client
#define ARM_UC_HTTP_RESUME_TEST_MESSAGES_ENABLE                               0                                                                                                 // set by library:update-client
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST                                     64                                                                                                // set by library:update-client
#define BS_PATCH_COMPILE_TIME_MEMORY_ALLOC                                    1024                                                                                              // set by library:delta-tool
// NOTE: Use MbedTLS configuration of FreeRTOS.
// #define MBEDTLS_USER_CONFIG_FILE                                              "mbedTLSConfig_mbedOS.h"                                                                          // set by library:client_app
#define MBED_BOOTLOADER_SIZE                                                  (32*1024)                                                                                         // set by application
// NOTE: Set from define_R*.txt
// #define MBED_CLOUD_CLIENT_UPDATE_STORAGE                                      ARM_UCP_FLASHIAP_BLOCKDEVICE                                                                      // set by application[K64F]
#define MBED_CONF_APP_BUTTON_PINNAME                                          BUTTON1                                                                                           // set by application
#define MBED_CONF_APP_DEVELOPER_MODE                                          1                                                                                                 // set by application
#define MBED_CONF_APP_LED_PINNAME                                             LED_RED                                                                                           // set by application
#define MBED_CONF_ATMEL_RF_ASSUME_SPACED_SPI                                  0                                                                                                 // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_FULL_SPI_SPEED                                     7500000                                                                                           // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_FULL_SPI_SPEED_BYTE_SPACING                        250                                                                                               // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_IRQ_THREAD_STACK_SIZE                              1024                                                                                              // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_LOW_SPI_SPEED                                      3750000                                                                                           // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_PROVIDE_DEFAULT                                    0                                                                                                 // set by library:atmel-rf
#define MBED_CONF_ATMEL_RF_USE_SPI_SPACING_API                                0                                                                                                 // set by library:atmel-rf
#define MBED_CONF_CELLULAR_CONTROL_PLANE_OPT                                  0                                                                                                 // set by library:cellular
#define MBED_CONF_CELLULAR_DEBUG_AT                                           0                                                                                                 // set by library:cellular
#define MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY                             0                                                                                                 // set by library:cellular
#define MBED_CONF_CELLULAR_USE_APN_LOOKUP                                     1                                                                                                 // set by library:cellular
#define MBED_CONF_CELLULAR_USE_SMS                                            1                                                                                                 // set by library:cellular
#define MBED_CONF_DRIVERS_QSPI_CSN                                            QSPI_FLASH1_CSN                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO0                                            QSPI_FLASH1_IO0                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO1                                            QSPI_FLASH1_IO1                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO2                                            QSPI_FLASH1_IO2                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_IO3                                            QSPI_FLASH1_IO3                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_QSPI_SCK                                            QSPI_FLASH1_SCK                                                                                   // set by library:drivers
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE                              256                                                                                               // set by library:drivers
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE                              256                                                                                               // set by library:drivers
#define MBED_CONF_ESP8266_DEBUG                                               0                                                                                                 // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_OFF_TIME_MS                                   3                                                                                                 // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_ON_POLARITY                                   0                                                                                                 // set by library:esp8266
#define MBED_CONF_ESP8266_POWER_ON_TIME_MS                                    3                                                                                                 // set by library:esp8266
#define MBED_CONF_ESP8266_PROVIDE_DEFAULT                                     0                                                                                                 // set by library:esp8266
#define MBED_CONF_ESP8266_SERIAL_BAUDRATE                                     115200                                                                                            // set by library:esp8266
#define MBED_CONF_ESP8266_SOCKET_BUFSIZE                                      8192                                                                                              // set by library:esp8266
#define MBED_CONF_EVENTS_PRESENT                                              1                                                                                                 // set by library:events
#define MBED_CONF_EVENTS_SHARED_DISPATCH_FROM_APPLICATION                     0                                                                                                 // set by library:events
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE                                     2048                                                                                              // set by application[*]
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE                            256                                                                                               // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE                            1024                                                                                              // set by library:events
#define MBED_CONF_EVENTS_SHARED_STACKSIZE                                     2048                                                                                              // set by application[*]
#define MBED_CONF_EVENTS_USE_LOWPOWER_TIMER_TICKER                            0                                                                                                 // set by library:events
#define MBED_CONF_FAT_CHAN_FFS_DBG                                            0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_CODE_PAGE                                       437                                                                                               // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_EXFAT                                        0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_HEAPBUF                                      1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_LOCK                                         0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_MINIMIZE                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_NOFSINFO                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_NORTC                                        0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_READONLY                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_REENTRANT                                    0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_RPATH                                        1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_TIMEOUT                                      1000                                                                                              // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_FS_TINY                                         1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_LFN_BUF                                         255                                                                                               // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_LFN_UNICODE                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MAX_LFN                                         255                                                                                               // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MAX_SS                                          4096                                                                                              // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MIN_SS                                          512                                                                                               // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_MULTI_PARTITION                                 0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_MDAY                                      1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_MON                                       1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_NORTC_YEAR                                      2017                                                                                              // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_SFN_BUF                                         12                                                                                                // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_STRF_ENCODE                                     3                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_STR_VOLUME_ID                                   0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_SYNC_T                                          HANDLE                                                                                            // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_CHMOD                                       0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_EXPAND                                      0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FASTSEEK                                    0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FIND                                        0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_FORWARD                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_LABEL                                       0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_LFN                                         3                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_MKFS                                        1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_STRFUNC                                     0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_USE_TRIM                                        1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_VOLUMES                                         4                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FF_VOLUME_STRS                                     "RAM","NAND","CF","SD","SD2","USB","USB2","USB3"                                                  // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FLUSH_ON_NEW_CLUSTER                               0                                                                                                 // set by library:fat_chan
#define MBED_CONF_FAT_CHAN_FLUSH_ON_NEW_SECTOR                                1                                                                                                 // set by library:fat_chan
#define MBED_CONF_FILESYSTEM_PRESENT                                          1                                                                                                 // set by library:filesystem
#define MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS                          0xFFFFFFFF                                                                                        // set by library:flashiap-block-device
#define MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE                                  0                                                                                                 // set by library:flashiap-block-device
#define MBED_CONF_GEMALTO_CINTERION_BAUDRATE                                  115200                                                                                            // set by library:GEMALTO_CINTERION
#define MBED_CONF_GEMALTO_CINTERION_PROVIDE_DEFAULT                           0                                                                                                 // set by library:GEMALTO_CINTERION
#define MBED_CONF_GENERIC_AT3GPP_BAUDRATE                                     115200                                                                                            // set by library:GENERIC_AT3GPP
#define MBED_CONF_GENERIC_AT3GPP_PROVIDE_DEFAULT                              0                                                                                                 // set by library:GENERIC_AT3GPP
#define MBED_CONF_KINETIS_EMAC_RX_RING_LEN                                    2                                                                                                 // set by library:kinetis-emac
#define MBED_CONF_KINETIS_EMAC_TX_RING_LEN                                    1                                                                                                 // set by library:kinetis-emac
#define MBED_CONF_LORA_ADR_ON                                                 1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_APPLICATION_EUI                                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                                                  // set by library:lora
#define MBED_CONF_LORA_APPLICATION_KEY                                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // set by library:lora
#define MBED_CONF_LORA_APPSKEY                                                {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // set by library:lora
#define MBED_CONF_LORA_APP_PORT                                               15                                                                                                // set by library:lora
#define MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE                               1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_DEVICE_ADDRESS                                         0x00000000                                                                                        // set by library:lora
#define MBED_CONF_LORA_DEVICE_EUI                                             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                                                  // set by library:lora
#define MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH                               5                                                                                                 // set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON                                          1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON_JOIN                                     1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_FSB_MASK                                               {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}                                                          // set by library:lora
#define MBED_CONF_LORA_FSB_MASK_CHINA                                         {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}                                                  // set by library:lora
#define MBED_CONF_LORA_LBT_ON                                                 0                                                                                                 // set by library:lora
#define MBED_CONF_LORA_MAX_SYS_RX_ERROR                                       5                                                                                                 // set by library:lora
#define MBED_CONF_LORA_NB_TRIALS                                              12                                                                                                // set by library:lora
#define MBED_CONF_LORA_NWKSKEY                                                {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // set by library:lora
#define MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION                                1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_PHY                                                    EU868                                                                                             // set by library:lora
#define MBED_CONF_LORA_PUBLIC_NETWORK                                         1                                                                                                 // set by library:lora
#define MBED_CONF_LORA_TX_MAX_SIZE                                            64                                                                                                // set by library:lora
#define MBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH                                 8                                                                                                 // set by library:lora
#define MBED_CONF_LORA_WAKEUP_TIME                                            5                                                                                                 // set by library:lora
#define MBED_CONF_LWIP_ADDR_TIMEOUT                                           5                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_ADDR_TIMEOUT_MODE                                      1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_DEBUG_ENABLED                                          0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_DEFAULT_TCP_RECVMBOX_SIZE                              8                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_DEFAULT_THREAD_STACKSIZE                               512                                                                                               // set by library:lwip
#define MBED_CONF_LWIP_DHCP_TIMEOUT                                           60                                                                                                // set by library:lwip
#define MBED_CONF_LWIP_ENABLE_PPP_TRACE                                       0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_ETHERNET_ENABLED                                       1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_IPV4_ENABLED                                           1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_IPV6_ENABLED                                           0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_IP_VER_PREF                                            4                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_L3IP_ENABLED                                           0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_MBOX_SIZE                                              8                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_MEMP_NUM_TCPIP_MSG_INPKT                               8                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_MEMP_NUM_TCP_SEG                                       16                                                                                                // set by library:lwip
#define MBED_CONF_LWIP_MEM_SIZE                                               12500                                                                                             // set by application[Freescale]
#define MBED_CONF_LWIP_NUM_NETBUF                                             8                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_NUM_PBUF                                               8                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_PBUF_POOL_SIZE                                         5                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_PPP_ENABLED                                            1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_PPP_IPV4_ENABLED                                       0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_PPP_IPV6_ENABLED                                       0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_PPP_THREAD_STACKSIZE                                   768                                                                                               // set by library:lwip
#define MBED_CONF_LWIP_PRESENT                                                1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_RAW_SOCKET_ENABLED                                     0                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_SOCKET_MAX                                             4                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCPIP_MBOX_SIZE                                        8                                                                                                 // set by library:lwip
// #define MBED_CONF_LWIP_TCPIP_THREAD_PRIORITY                                  osPriorityNormal                                                                                  // set by library:lwip
#define MBED_CONF_LWIP_TCPIP_THREAD_STACKSIZE                                 1200                                                                                              // set by library:lwip
#define MBED_CONF_LWIP_TCP_CLOSE_TIMEOUT                                      1000                                                                                              // set by library:lwip
#define MBED_CONF_LWIP_TCP_ENABLED                                            1                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCP_MAXRTX                                             6                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCP_MSS                                                536                                                                                               // set by library:lwip
#define MBED_CONF_LWIP_TCP_SERVER_MAX                                         4                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCP_SND_BUF                                            (2 * TCP_MSS)                                                                                     // set by library:lwip
#define MBED_CONF_LWIP_TCP_SOCKET_MAX                                         4                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCP_SYNMAXRTX                                          6                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_TCP_WND                                                (4 * TCP_MSS)                                                                                     // set by library:lwip
#define MBED_CONF_LWIP_UDP_SOCKET_MAX                                         4                                                                                                 // set by library:lwip
#define MBED_CONF_LWIP_USE_MBED_TRACE                                         0                                                                                                 // set by library:lwip
#define MBED_CONF_MBED_CLIENT_ENABLE_OBSERVATION_PARAMETERS                   1                                                                                                 // set by library:mbed-client
//#define MBED_CONF_MBED_CLIENT_EVENT_LOOP_SIZE                                 1024                                                                                              // set by library:mbed-client
#define MBED_CONF_MBED_CLIENT_PAL_RSA_REQUIRED                                0                                                                                                 // set by library:mbed-client-pal
#define MBED_CONF_MBED_CLIENT_RECONNECTION_COUNT                              3                                                                                                 // set by library:mbed-client
#define MBED_CONF_MBED_CLIENT_RECONNECTION_INTERVAL                           5                                                                                                 // set by library:mbed-client
//#define MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_MSGS_COUNT              5                                                                                                 // set by library:mbed-client
#define MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_MSGS               5                                                                                                 // set by library:mbed-client
#define MBED_CONF_MBED_CLIENT_TCP_KEEPALIVE_INTERVAL                          540                                                                                               // set by library:mbed-client
#define MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT            1                                                                                                 // set by library:mbed-cloud-client
#define MBED_CONF_MBED_CLOUD_CLIENT_EXTERNAL_SST_SUPPORT                      1                                                                                                 // set by library:mbed-cloud-client
#define MBED_CONF_MBED_CLOUD_CLIENT_UPDATE_DOWNLOAD_PROTOCOL                  MBED_CLOUD_CLIENT_UPDATE_DOWNLOAD_PROTOCOL_COAP                                                   // set by library:mbed-cloud-client
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL                            0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL_MASK                       0x7fff800                                                                                         // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL_PAGE                       0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_DEVICE_TYPE                        NET_6LOWPAN_ROUTER                                                                                // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PANID_FILTER                       0xffff                                                                                            // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PSK_KEY                            {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf}  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PSK_KEY_ID                         1                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_SECURITY_MODE                      NONE                                                                                              // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_6LOWPAN_ND_SEC_LEVEL                          5                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_HEAP_SIZE                                     32500                                                                                             // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_HEAP_STAT_INFO                                NULL                                                                                              // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_CHANNEL                         22                                                                                                // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_CHANNEL_MASK                    0x7fff800                                                                                         // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_CHANNEL_PAGE                    0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_COMMISSIONING_DATASET_TIMESTAMP 0x10000                                                                                           // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_EXTENDED_PANID                  {0xf1, 0xb5, 0xa1, 0xb2,0xc4, 0xd5, 0xa1, 0xbd }                                                  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_ML_PREFIX                       {0xfd, 0x0, 0x0d, 0xb8, 0x0, 0x0, 0x0, 0x0}                                                       // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_NETWORK_NAME                    "Thread Network"                                                                                  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_PANID                           0x0700                                                                                            // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_CONFIG_PSKC                            {0xc8, 0xa6, 0x2e, 0xae, 0xf3, 0x68, 0xf3, 0x46, 0xa9, 0x9e, 0x57, 0x85, 0x98, 0x9d, 0x1c, 0xd0}  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_DEVICE_TYPE                            MESH_DEVICE_TYPE_THREAD_ROUTER                                                                    // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_MASTER_KEY                             {0x10, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_PSKD                                   "ABCDEFGH"                                                                                        // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_SECURITY_POLICY                        255                                                                                               // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_THREAD_USE_STATIC_LINK_CONFIG                 1                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_USE_MALLOC_FOR_HEAP                           0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_BC_CHANNEL_FUNCTION                     255                                                                                               // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_BC_DWELL_INTERVAL                       0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_BC_FIXED_CHANNEL                        0xffff                                                                                            // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_BC_INTERVAL                             0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME                            "Wi-SUN Network"                                                                                  // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_OPERATING_CLASS                         255                                                                                               // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_OPERATING_MODE                          255                                                                                               // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_REGULATORY_DOMAIN                       3                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_UC_CHANNEL_FUNCTION                     255                                                                                               // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_UC_DWELL_INTERVAL                       0                                                                                                 // set by library:mbed-mesh-api
#define MBED_CONF_MBED_MESH_API_WISUN_UC_FIXED_CHANNEL                        0xffff                                                                                            // set by library:mbed-mesh-api
#define MBED_CONF_MCR20A_PROVIDE_DEFAULT                                      0                                                                                                 // set by library:mcr20a
#define MBED_CONF_NANOSTACK_CONFIGURATION                                     nanostack_full                                                                                    // set by library:nanostack
#define MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT        0                                                                                                 // set by library:nanostack-hal
#define MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_DISPATCH_FROM_APPLICATION          0                                                                                                 // set by library:nanostack-hal
#define MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_THREAD_STACK_SIZE                  6144                                                                                              // set by library:nanostack-hal
#define MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_USE_MBED_EVENTS                    0                                                                                                 // set by library:nanostack-hal
#define MBED_CONF_NSAPI_DEFAULT_MESH_TYPE                                     THREAD                                                                                            // set by library:nsapi
#define MBED_CONF_NSAPI_DEFAULT_STACK                                         LWIP                                                                                              // set by library:nsapi
#define MBED_CONF_NSAPI_DEFAULT_WIFI_PASSWORD                                 "Password"                                                                                        // set by application[*]
#define MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY                                 WPA_WPA2                                                                                          // set by application[*]
#define MBED_CONF_NSAPI_DEFAULT_WIFI_SSID                                     "SSID"                                                                                            // set by application[*]
#define MBED_CONF_NSAPI_DNS_CACHE_SIZE                                        3                                                                                                 // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME                                10000                                                                                             // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_RETRIES                                           1                                                                                                 // set by library:nsapi
#define MBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS                                    10                                                                                                // set by library:nsapi
#define MBED_CONF_NSAPI_PRESENT                                               1                                                                                                 // set by library:nsapi
#define MBED_CONF_NSAPI_SOCKET_STATS_ENABLED                                  0                                                                                                 // set by library:nsapi
#define MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT                                10                                                                                                // set by library:nsapi
#define MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED                              1                                                                                                 // set by library:platform[K64F]
#define MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX                                   8                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE                           9600                                                                                              // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO                             0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED                     0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_ENABLED                                 0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE                                    4                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_REBOOT_MAX                                   1                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_FATAL_ERROR_AUTO_REBOOT_ENABLED                    1                                                                                                 // set by library:platform[K64F]
#define MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR                           0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN                             16                                                                                                // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT                       1                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT               0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS     6                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER                            0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE                                    115200                                                                                            // set by application[*]
#define MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL                              1                                                                                                 // set by application[*]
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES                             1                                                                                                 // set by application[*]
#define MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES                         0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT                                1                                                                                                 // set by application[*]
#define MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY                         0                                                                                                 // set by library:platform
#define MBED_CONF_PLATFORM_USE_MPU                                            1                                                                                                 // set by library:platform
#define MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP                                   1                                                                                                 // set by library:ppp-cell-iface
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE                        256                                                                                               // set by library:ppp-cell-iface
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT                            8000                                                                                              // set by library:ppp-cell-iface
#define MBED_CONF_PPP_CELL_IFACE_BAUD_RATE                                    115200                                                                                            // set by library:ppp-cell-iface
#define MBED_CONF_PPP_ENABLED                                                 0                                                                                                 // set by library:ppp
#define MBED_CONF_PPP_ENABLE_TRACE                                            0                                                                                                 // set by library:ppp
#define MBED_CONF_PPP_IPV4_ENABLED                                            1                                                                                                 // set by library:ppp
#define MBED_CONF_PPP_IPV6_ENABLED                                            0                                                                                                 // set by library:ppp
#define MBED_CONF_PPP_MBED_EVENT_QUEUE                                        0                                                                                                 // set by library:ppp
#define MBED_CONF_PPP_THREAD_STACKSIZE                                        816                                                                                               // set by library:ppp
#define MBED_CONF_PSA_PRESENT                                                 1                                                                                                 // set by library:psa
#define MBED_CONF_QUECTEL_BC95_BAUDRATE                                       9600                                                                                              // set by library:QUECTEL_BC95
#define MBED_CONF_QUECTEL_BC95_PROVIDE_DEFAULT                                0                                                                                                 // set by library:QUECTEL_BC95
#define MBED_CONF_QUECTEL_BG96_BAUDRATE                                       115200                                                                                            // set by library:QUECTEL_BG96
#define MBED_CONF_QUECTEL_BG96_PROVIDE_DEFAULT                                0                                                                                                 // set by library:QUECTEL_BG96
#define MBED_CONF_QUECTEL_EC2X_BAUDRATE                                       115200                                                                                            // set by library:QUECTEL_EC2X
#define MBED_CONF_QUECTEL_EC2X_PROVIDE_DEFAULT                                0                                                                                                 // set by library:QUECTEL_EC2X
#define MBED_CONF_QUECTEL_M26_BAUDRATE                                        115200                                                                                            // set by library:QUECTEL_M26
#define MBED_CONF_QUECTEL_M26_PROVIDE_DEFAULT                                 0                                                                                                 // set by library:QUECTEL_M26
#define MBED_CONF_QUECTEL_UG96_BAUDRATE                                       115200                                                                                            // set by library:QUECTEL_UG96
#define MBED_CONF_QUECTEL_UG96_PROVIDE_DEFAULT                                0                                                                                                 // set by library:QUECTEL_UG96
#define MBED_CONF_RM1000_AT_BAUDRATE                                          230400                                                                                            // set by library:RM1000_AT
#define MBED_CONF_RM1000_AT_PROVIDE_DEFAULT                                   0                                                                                                 // set by library:RM1000_AT
#define MBED_CONF_RTOS_API_PRESENT                                            1                                                                                                 // set by library:rtos-api
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE                                 512                                                                                               // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_DEBUG_EXTRA                     0                                                                                                 // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_TICKLESS_EXTRA                  256                                                                                               // set by library:rtos
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE                                 5120                                                                                              // set by application[*]
#define MBED_CONF_RTOS_PRESENT                                                1                                                                                                 // set by library:rtos
#define MBED_CONF_RTOS_THREAD_STACK_SIZE                                      4096                                                                                              // set by library:rtos
#define MBED_CONF_RTOS_TIMER_THREAD_STACK_SIZE                                768                                                                                               // set by library:rtos
#define MBED_CONF_S2LP_PROVIDE_DEFAULT                                        0                                                                                                 // set by library:s2lp
#define MBED_CONF_SARA4_PPP_BAUDRATE                                          115200                                                                                            // set by library:SARA4_PPP
#define MBED_CONF_SARA4_PPP_PROVIDE_DEFAULT                                   0                                                                                                 // set by library:SARA4_PPP
#define MBED_CONF_SD_CMD0_IDLE_STATE_RETRIES                                  5                                                                                                 // set by library:sd
#define MBED_CONF_SD_CMD_TIMEOUT                                              10000                                                                                             // set by library:sd
#define MBED_CONF_SD_CRC_ENABLED                                              1                                                                                                 // set by library:sd
#define MBED_CONF_SD_FSFAT_SDCARD_INSTALLED                                   1                                                                                                 // set by library:sd
#define MBED_CONF_SD_INIT_FREQUENCY                                           100000                                                                                            // set by library:sd
#define MBED_CONF_SD_SPI_CLK                                                  SPI_SCK                                                                                           // set by library:sd
#define MBED_CONF_SD_SPI_CS                                                   SPI_CS                                                                                            // set by library:sd
#define MBED_CONF_SD_SPI_MISO                                                 SPI_MISO                                                                                          // set by library:sd
#define MBED_CONF_SD_SPI_MOSI                                                 SPI_MOSI                                                                                          // set by library:sd
#define MBED_CONF_SD_TEST_BUFFER                                              8192                                                                                              // set by library:sd
#define MBED_CONF_STORAGE_DEFAULT_KV                                          kv                                                                                                // set by library:storage
#define MBED_CONF_STORAGE_FILESYSTEM_BLOCKDEVICE                              SD                                                                                                // set by application[K64F]
#define MBED_CONF_STORAGE_FILESYSTEM_EXTERNAL_BASE_ADDRESS                    (0x0)                                                                                             // set by application[K64F]
#define MBED_CONF_STORAGE_FILESYSTEM_EXTERNAL_SIZE                            (1024*1024*64)                                                                                    // set by application[K64F]
#define MBED_CONF_STORAGE_FILESYSTEM_FILESYSTEM                               LITTLE                                                                                            // set by application[K64F]
#define MBED_CONF_STORAGE_FILESYSTEM_FOLDER_PATH                              kvstore                                                                                           // set by library:storage_filesystem
#define MBED_CONF_STORAGE_FILESYSTEM_INTERNAL_BASE_ADDRESS                    (32*1024)                                                                                         // set by application[K64F]
#define MBED_CONF_STORAGE_FILESYSTEM_MOUNT_POINT                              kv                                                                                                // set by library:storage_filesystem
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_BLOCKDEVICE                       default                                                                                           // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_EXTERNAL_BASE_ADDRESS             0                                                                                                 // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_EXTERNAL_SIZE                     0                                                                                                 // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_FILESYSTEM                        default                                                                                           // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_FOLDER_PATH                       kvstore                                                                                           // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_NO_RBP_MOUNT_POINT                       kv                                                                                                // set by library:storage_filesystem_no_rbp
#define MBED_CONF_STORAGE_FILESYSTEM_RBP_INTERNAL_SIZE                        (8*1024)                                                                                          // set by application[K64F]
//#define MBED_CONF_STORAGE_STORAGE_TYPE                                        FILESYSTEM                                                                                        // set by application[K64F]
#define MBED_CONF_STORAGE_TDB_EXTERNAL_BLOCKDEVICE                            default                                                                                           // set by library:storage_tdb_external
#define MBED_CONF_STORAGE_TDB_EXTERNAL_EXTERNAL_BASE_ADDRESS                  0                                                                                                 // set by library:storage_tdb_external
#define MBED_CONF_STORAGE_TDB_EXTERNAL_EXTERNAL_SIZE                          0                                                                                                 // set by library:storage_tdb_external
#define MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS                  0                                                                                                 // set by library:storage_tdb_external
#define MBED_CONF_STORAGE_TDB_EXTERNAL_NO_RBP_BLOCKDEVICE                     default                                                                                           // set by library:storage_tdb_external_no_rbp
#define MBED_CONF_STORAGE_TDB_EXTERNAL_NO_RBP_EXTERNAL_BASE_ADDRESS           0                                                                                                 // set by library:storage_tdb_external_no_rbp
#define MBED_CONF_STORAGE_TDB_EXTERNAL_NO_RBP_EXTERNAL_SIZE                   0                                                                                                 // set by library:storage_tdb_external_no_rbp
#define MBED_CONF_STORAGE_TDB_EXTERNAL_RBP_INTERNAL_SIZE                      0                                                                                                 // set by library:storage_tdb_external
// #define MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS                  0                                                                                                 // set by library:storage_tdb_internal
// #define MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE                          0                                                                                                 // set by library:storage_tdb_internal
#define MBED_CONF_TARGET_BOOT_STACK_SIZE                                      0x400                                                                                             // set by library:rtos[*]
#define MBED_CONF_TARGET_CONSOLE_UART                                         1                                                                                                 // set by target:Target
#define MBED_CONF_TARGET_DEEP_SLEEP_LATENCY                                   0                                                                                                 // set by target:Target
#define MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT                               0                                                                                                 // set by target:Target
#define MBED_CONF_TARGET_MPU_ROM_END                                          0x0fffffff                                                                                        // set by target:Target
#define MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE                       ETHERNET                                                                                          // set by application[K64F]
#define MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER                              0                                                                                                 // set by target:Target
#define MBED_CONF_TARGET_XIP_ENABLE                                           0                                                                                                 // set by target:Target
#define MBED_CONF_TELIT_HE910_BAUDRATE                                        115200                                                                                            // set by library:TELIT_HE910
#define MBED_CONF_TELIT_HE910_PROVIDE_DEFAULT                                 0                                                                                                 // set by library:TELIT_HE910
#define MBED_CONF_TELIT_ME910_BAUDRATE                                        115200                                                                                            // set by library:TELIT_ME910
#define MBED_CONF_TELIT_ME910_PROVIDE_DEFAULT                                 0                                                                                                 // set by library:TELIT_ME910
#define MBED_CONF_UBLOX_AT_BAUDRATE                                           115200                                                                                            // set by library:UBLOX_AT
#define MBED_CONF_UBLOX_AT_PROVIDE_DEFAULT                                    0                                                                                                 // set by library:UBLOX_AT
#define MBED_CONF_UBLOX_N2XX_BAUDRATE                                         9600                                                                                              // set by library:UBLOX_N2XX
#define MBED_CONF_UBLOX_N2XX_PROVIDE_DEFAULT                                  0                                                                                                 // set by library:UBLOX_N2XX
#define MBED_CONF_UBLOX_PPP_BAUDRATE                                          115200                                                                                            // set by library:UBLOX_PPP
#define MBED_CONF_UBLOX_PPP_PROVIDE_DEFAULT                                   0                                                                                                 // set by library:UBLOX_PPP
#define MBED_CONF_UPDATE_CLIENT_APPLICATION_DETAILS                           (40*1024)                                                                                         // set by application[K64F]
#define MBED_CONF_UPDATE_CLIENT_BOOTLOADER_DETAILS                            0x00007188                                                                                        // set by application[K64F]
#define MBED_CONF_UPDATE_CLIENT_FIRMWARE_HEADER_VERSION                       0                                                                                                 // set by library:update-client
// #define MBED_CONF_UPDATE_CLIENT_STORAGE_ADDRESS                               (1024*1024*64)                                                                                    // set by application[K64F]
#define MBED_CONF_UPDATE_CLIENT_STORAGE_LOCATIONS                             1                                                                                                 // set by application[*]
#define MBED_CONF_UPDATE_CLIENT_STORAGE_PAGE                                  8                                                                                                 // set by library:update-client
//#define MBED_CONF_UPDATE_CLIENT_STORAGE_SIZE                                  ((MBED_ROM_START + MBED_ROM_SIZE - APPLICATION_ADDR) * MBED_CONF_UPDATE_CLIENT_STORAGE_LOCATIONS) // set by application[K64F]
#define MBED_CONF_WIZFI310_CTS                                                NC                                                                                                // set by library:wizfi310
#define MBED_CONF_WIZFI310_DEBUG                                              0                                                                                                 // set by library:wizfi310
#define MBED_CONF_WIZFI310_PROVIDE_DEFAULT                                    0                                                                                                 // set by library:wizfi310
#define MBED_CONF_WIZFI310_RTS                                                NC                                                                                                // set by library:wizfi310
#define MBED_CONF_WIZFI310_RX                                                 NC                                                                                                // set by library:wizfi310
#define MBED_CONF_WIZFI310_TX                                                 NC                                                                                                // set by library:wizfi310
#define MBED_LFS_BLOCK_SIZE                                                   512                                                                                               // set by library:littlefs
#define MBED_LFS_ENABLE_INFO                                                  0                                                                                                 // set by library:littlefs
#define MBED_LFS_INTRINSICS                                                   1                                                                                                 // set by library:littlefs
#define MBED_LFS_LOOKAHEAD                                                    512                                                                                               // set by library:littlefs
#define MBED_LFS_PROG_SIZE                                                    64                                                                                                // set by library:littlefs
#define MBED_LFS_READ_SIZE                                                    64                                                                                                // set by library:littlefs
#define MBED_STACK_DUMP_ENABLED                                               0                                                                                                 // set by library:platform
#define MEM_ALLOC                                                             malloc                                                                                            // set by library:mbed-trace
#define MEM_FREE                                                              free                                                                                              // set by library:mbed-trace
#define NVSTORE_ENABLED                                                       1                                                                                                 // set by library:nvstore
#define NVSTORE_MAX_KEYS                                                      16                                                                                                // set by library:nvstore
//#define PAL_DNS_API_VERSION                                                   2                                                                                                 // set by library:mbed-client-pal
#define PAL_DTLS_PEER_MIN_TIMEOUT                                             5000                                                                                              // set by library:client_app
#define PAL_MAX_FRAG_LEN                                                      0                                                                                                 // set by library:mbed-client-pal
//#define PAL_NET_SERVER_SOCKET_API                                             1                                                                                                 // set by library:mbed-client-pal
#define PAL_STATIC_MEMBUF_SIZE_FOR_MBEDTLS                                    14000                                                                                             // set by library:mbed-client-pal
// NOTE: Do not use Mbed PAL config.
// #define PAL_USER_DEFINED_CONFIGURATION                                        "pal_config_MbedOS.h"                                                                             // set by library:client_app
#define PLATFORM_ENABLE_BUTTON                                                1                                                                                                 // set by library:client_app
#define PLATFORM_ENABLE_LED                                                   1                                                                                                 // set by library:client_app
#define PPP_DEBUG                                                             0                                                                                                 // set by library:ppp
// Macros
#define ARM_UC_PROFILE_MBED_CLOUD_CLIENT                                      1                                                                                                 // defined by library:mbed-cloud-client
#define ARM_UC_USE_PAL_BLOCKDEVICE                                            1                                                                                                 // defined by application
#define MBEDTLS_CIPHER_MODE_CTR                                                                                                                                                 // defined by library:SecureStore
#define MBEDTLS_CMAC_C                                                                                                                                                          // defined by library:SecureStore
#define MBEDTLS_PSA_HAS_ITS_IO                                                                                                                                                  // defined by library:mbed-crypto
// NOTE: Use FreeRTOS PDMC config.
// #define MBED_CLIENT_USER_CONFIG_FILE                                          "mbed_cloud_client_user_config.h"                                                                 // defined by library:client_app
// #define MBED_CLOUD_CLIENT_USER_CONFIG_FILE                                    "mbed_cloud_client_user_config.h"                                                                 // defined by library:client_app
#define NSAPI_PPP_AVAILABLE                                                   (MBED_CONF_PPP_ENABLED || MBED_CONF_LWIP_PPP_ENABLED)                                             // defined by library:ppp
#define NS_USE_EXTERNAL_MBED_TLS                                                                                                                                                // defined by library:nanostack
#define SN_COAP_REDUCE_BLOCKWISE_HEAP_FOOTPRINT                               1                                                                                                 // defined by library:mbed-cloud-client
#define UNITY_INCLUDE_CONFIG_H                                                                                                                                                  // defined by library:utest
#define _RTE_                                                                                                                                                                   // defined by library:rtos

#endif
