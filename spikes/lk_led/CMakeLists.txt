# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_led)

target_include_directories(spike_lk_led
	PRIVATE
		.
)

target_sources(spike_lk_led
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_led
	driver_FastLED
)

target_link_custom_leka_targets(spike_lk_led)
