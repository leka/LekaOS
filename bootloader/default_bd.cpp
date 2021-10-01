// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "QSPIFBlockDevice.h"
#include "SlicingBlockDevice.h"

mbed::BlockDevice *get_secondary_bd(void)
{
	// In this case, our flash is much larger than a single image so
	// slice it into the size of an image slot

	static QSPIFBlockDevice _bd;

	static mbed::SlicingBlockDevice sliced_bd(&_bd, 0x0, MCUBOOT_SLOT_SIZE);
	return &sliced_bd;
}
