
// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LEKA_PIN_NAMES_H_
#define _LEKA_OS_LEKA_PIN_NAMES_H_

#include "PinNames.h"

// LekaPinNames.h v1.1.0
// Generated on 2020/09/15

constexpr PinName BATTERY_BMS_I2C_SCL          = PB_6;
constexpr PinName BATTERY_BMS_I2C_SDA          = PB_7;

constexpr PinName BLE_IRQ                      = PI_11;
constexpr PinName BLE_RESET                    = PE_3;
constexpr PinName BLE_SPI_MISO                 = PF_8;
constexpr PinName BLE_SPI_MOSI                 = PF_9;
constexpr PinName BLE_SPI_NSS                  = PF_6;
constexpr PinName BLE_SPI_SCK                  = PF_7;

constexpr PinName BT_RESET                     = PC_8;
constexpr PinName BT_UART_RX                   = PC_7;
constexpr PinName BT_UART_TX                   = PC_6;
constexpr PinName BT_WAKE_UP                   = PG_7;

constexpr PinName HSE_IN                       = PH_0;
constexpr PinName HSE_OUT                      = PH_1;

constexpr PinName LED_BELT_SPI_MOSI            = PG_14;
constexpr PinName LED_BELT_SPI_SCK             = PG_13;
constexpr PinName LED_EARS_SPI_MOSI            = PB_15;
constexpr PinName LED_EARS_SPI_SCK             = PB_13;

constexpr PinName LSE_IN                       = PC_14;
constexpr PinName LSE_OUT                      = PC_15;

constexpr PinName MCU_MIC_INPUT                = PF_10;
constexpr PinName MCU_SOUND_OUT                = PA_4;
constexpr PinName MCU_SYS_WAKE_UP              = PA_0;

constexpr PinName MOTOR_LEFT_PWM               = PA_1;
constexpr PinName MOTOR_RIGHT_DIRECTION_1      = PE_4;
constexpr PinName MOTOR_RIGHT_DIRECTION_2      = PE_5;
constexpr PinName MOTOR_RIGHT_PWM              = PA_2;

constexpr PinName QSPI_FLASH_CLK               = PB_2;
constexpr PinName QSPI_FLASH_IO0               = PD_11;
constexpr PinName QSPI_FLASH_IO1               = PC_10;
constexpr PinName QSPI_FLASH_IO2               = PE_2;
constexpr PinName QSPI_FLASH_IO3               = PD_13;
constexpr PinName QSPI_FLASH_nCS               = PB_10;
constexpr PinName QSPI_FLASH_nCS_1             = PD_12;
constexpr PinName QSPI_FLASH_nCS_2             = PH_6;
constexpr PinName QSPI_FLASH_nCS_3             = PJ_4;

constexpr PinName RFID_UART_RX                 = PA_11;
constexpr PinName RFID_UART_TX                 = PA_12;

constexpr PinName SCREEN_BACKLIGHT_PWM         = PB_14;

constexpr PinName SD_SPI_CS                    = PC_4;
constexpr PinName SD_SPI_MISO                  = PA_6;
constexpr PinName SD_SPI_MOSI                  = PA_7;
constexpr PinName SD_SPI_SCK                   = PA_5;

constexpr PinName SENSOR_IMU_IRQ               = PG_9;
constexpr PinName SENSOR_IMU_TH_I2C_SCL        = PB_8;
constexpr PinName SENSOR_IMU_TH_I2C_SDA        = PB_9;
constexpr PinName SENSOR_LIGHT_ADC_INPUT       = PB_1;
constexpr PinName SENSOR_MAGNETO_IRQ           = PA_15;
constexpr PinName SENSOR_PROXIMITY_MUX_I2C_SCL = PH_7;
constexpr PinName SENSOR_PROXIMITY_MUX_I2C_SDA = PC_9;
constexpr PinName SENSOR_PROXIMITY_MUX_IRQA    = PI_13;
constexpr PinName SENSOR_PROXIMITY_MUX_IRQB    = PG_10;
constexpr PinName SENSOR_PROXIMITY_MUX_RESET   = PI_12;
constexpr PinName SENSOR_TEMPERATURE_IRQ       = PB_5;

constexpr PinName SOUND_ENABLE                 = PA_8;

constexpr PinName SYS_POWER_CHECKUP_3V3        = PJ_0;
constexpr PinName SYS_POWER_CHECKUP_5V         = PJ_1;
constexpr PinName SYS_PROG_SWCLK               = PA_14;
constexpr PinName SYS_PROG_SWDIO               = PA_13;
constexpr PinName SYS_PROG_SWO                 = PB_3;
constexpr PinName SYS_PROG_VCP_RX              = PA_10;
constexpr PinName SYS_PROG_VCP_TX              = PA_9;

constexpr PinName WIFI_ENABLE                  = PC_12;
constexpr PinName WIFI_RESET                   = PJ_3;
constexpr PinName WIFI_USART_CTS               = PD_3;
constexpr PinName WIFI_USART_RTS               = PD_4;
constexpr PinName WIFI_USART_RX                = PA_3;
constexpr PinName WIFI_USART_TX                = PD_5;

#endif // _LEKA_OS_LEKA_PIN_NAMES_H_
