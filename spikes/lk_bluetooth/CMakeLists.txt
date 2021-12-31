# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_bluetooth)

target_include_directories(spike_lk_bluetooth
	PRIVATE
		.
)

target_sources(spike_lk_bluetooth
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_bluetooth
	PUBLIC
		mbed-os-static
		lib_LekaBluetooth
)

mbed_set_post_build(spike_lk_bluetooth)
