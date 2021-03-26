# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_light_sensor)

target_sources(spike_lk_light_sensor
	PRIVATE
		main.cpp
)

target_include_directories(spike_lk_light_sensor
	PRIVATE
		.
)

target_link_libraries(spike_lk_light_sensor
		lib_HelloWorld
		LKCoreLightSensor
)

target_link_custom_leka_targets(spike_lk_light_sensor)
