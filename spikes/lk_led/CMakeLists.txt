# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_led)

target_include_directories(spike_lk_led
	PRIVATE
		.
)

target_sources(spike_lk_led
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_led
	PUBLIC
		mbed-os-static
		driver_FastLED
)
