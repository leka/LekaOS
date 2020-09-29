// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_QSPI_IS25LP016D_H_
#define _LEKA_OS_LIB_QSPI_IS25LP016D_H_

#include "mbed.h"

constexpr uint8_t CMD_READ	= 0x03;
constexpr uint8_t CMD_WRITE = 0x02;
constexpr uint8_t CMD_ERASE = 0x20;
constexpr uint8_t CMD_RDSR	= 0x5;
constexpr uint8_t CMD_WREN	= 0x6;
constexpr uint8_t CMD_RSTEN = 0x66;
constexpr uint8_t CMD_RST	= 0x99;

constexpr uint8_t STATUS_REG_SIZE = 2;
constexpr uint8_t BIT_WIP		  = 0x01;
constexpr uint8_t BIT_WEL		  = 0x02;

constexpr int ExtFlashSize = 0x00200000;

#endif
