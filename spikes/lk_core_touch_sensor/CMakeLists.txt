# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_core_touch_sensor)

target_include_directories(spike_lk_core_touch_sensor
	PRIVATE
	.
)

target_sources(spike_lk_core_touch_sensor
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_core_touch_sensor
	CoreTouchSensor
	CoreI2C
	CoreIOExpander
	CoreQDAC
	IOKit
)
