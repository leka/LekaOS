# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_led_kit)

target_include_directories(spike_lk_led_kit
	PRIVATE
		.
		include
)

target_sources(spike_lk_led_kit
	PRIVATE
		main.cpp
		source/FastLum.cpp
		source/FastTwo.cpp
)

target_link_libraries(spike_lk_led_kit
	CoreLED
	ColorKit
	LedKit
	CoreEventFlags
)

target_link_custom_leka_targets(spike_lk_led_kit)
