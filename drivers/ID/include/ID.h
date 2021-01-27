// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_ID_H_
#define _LEKA_OS_DRIVER_ID_H_

#include <stdint.h>

#define PTR_32BITS_REG(addr) (*(volatile uint32_t *)(addr))
#define U_ID_REG			 0x1ff0f420

#define MCU_ID_FRONT  uint32_t PTR_REG(U_ID_REG + 0x0)
#define MCU_ID_MIDDLE uint32_t PTR_REG(U_ID_REG + 0x4)
#define MCU_ID_BACK	  uint32_t PTR_REG(U_ID_REG + 0x8)

namespace leka {

typedef struct {
	uint32_t front;
	uint32_t middle;
	uint32_t back;
} mcu_id;	// ID is 96 bits length

mcu_id get_id();

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_ID_H_
