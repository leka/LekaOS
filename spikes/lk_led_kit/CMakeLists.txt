# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_led_kit)

target_include_directories(spike_lk_led_kit
	PRIVATE
	.
)

target_sources(spike_lk_led_kit
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_led_kit
	CoreLED
	ColorKit
	LedKit
)
