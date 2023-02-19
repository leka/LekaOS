# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_touch_sensor_kit)

target_include_directories(spike_lk_touch_sensor_kit
	PRIVATE
	.
)

target_sources(spike_lk_touch_sensor_kit
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_touch_sensor_kit
	CoreI2C
	CoreSPI
	CoreIOExpander
	IOKit
	CoreQDAC
	CoreTouchSensor
	TouchSensorKit
	CoreLED
	LedKit
)
