# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_bluetooth)

target_include_directories(spike_lk_bluetooth
	PRIVATE
		.
)

target_sources(spike_lk_bluetooth
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_bluetooth
	lib_LekaBluetooth
)

target_link_custom_leka_targets(spike_lk_bluetooth)
