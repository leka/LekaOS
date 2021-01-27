// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ID.h"

namespace leka {

/* Read U_ID register */
mcu_id get_id()
{
	mcu_id id;

	id.front  = MCU_ID_FRONT;
	id.middle = MCU_ID_MIDDLE;
	id.back	  = MCU_ID_BACK;

	return id;
}

}	// namespace leka
