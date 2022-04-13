# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_ble)

target_include_directories(spike_lk_ble
	PRIVATE
		.
)

target_sources(spike_lk_ble
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_ble
	BLEKit
	FileManagerKit
	ContainerKit
	CoreEventQueue
)

target_link_custom_leka_targets(spike_lk_ble)
