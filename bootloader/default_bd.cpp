// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "QSPIFBlockDevice.h"
#include "SlicingBlockDevice.h"

auto get_secondary_bd() -> mbed::BlockDevice *
{
	// In this case, our flash is much larger than a single image so
	// slice it into the size of an image slot

	static auto _bd = QSPIFBlockDevice {};

	static auto sliced_bd = mbed::SlicingBlockDevice {&_bd, 0x0, MCUBOOT_SLOT_SIZE};

	return &sliced_bd;
}
