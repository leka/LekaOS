# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_sensor_temperature)

target_sources(spike_lk_sensor_temperature
	PRIVATE
		main.cpp
)

target_include_directories(spike_lk_sensor_temperature
	PRIVATE
    	.
)

target_link_libraries(spike_lk_sensor_temperature
	lib_HelloWorld
	LKCoreTemperatureSensor
)

target_link_custom_leka_targets(spike_lk_sensor_temperature)
