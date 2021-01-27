// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ID.h"

namespace leka {

/* Read U_ID register */
mcu_id get_id()
{
	mcu_id id;

	id.front  = MMIO32(U_ID_REG + 0x0);
	id.middle = MMIO32(U_ID_REG + 0x4);
	id.back	  = MMIO32(U_ID_REG + 0x8);

	return id;
}

}	// namespace leka
