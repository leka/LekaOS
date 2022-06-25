# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_mbed_blinky)

target_include_directories(spike_mbed_blinky
	PRIVATE
	.
)

target_sources(spike_mbed_blinky
	PRIVATE
	main.cpp
)

target_link_custom_leka_targets(spike_mbed_blinky)
