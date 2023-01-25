# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_lcd)

target_include_directories(spike_lk_lcd
	PRIVATE
	.
)

target_sources(spike_lk_lcd
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_lcd
	CoreVideo
	CoreLL
	CoreSTM32Hal
	CoreEventFlags
	EventLoopKit
	FileManagerKit
	VideoKit
)

target_link_custom_leka_targets(spike_lk_lcd)
