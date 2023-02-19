# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_mbed_blinky)

target_include_directories(spike_mbed_blinky
	PRIVATE
	.
)

target_sources(spike_mbed_blinky
	PRIVATE
	main.cpp
)
