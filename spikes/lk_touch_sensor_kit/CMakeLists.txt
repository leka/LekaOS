# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_touch_sensor_kit

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		CoreI2C
		CoreIOExpander
		CoreLED
		CoreQDAC
		CoreSPI
		CoreTouchSensor
		IOKit
		LedKit
		TouchSensorKit
)
