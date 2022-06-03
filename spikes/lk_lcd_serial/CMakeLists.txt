# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_lcd_serial)

target_include_directories(spike_lk_lcd_serial
	PRIVATE
		.
)

target_sources(spike_lk_lcd_serial
	PRIVATE
		main.cpp
		SerialCommunication.cpp
)

target_link_libraries(spike_lk_lcd_serial
	CoreVideo
	CoreLL
	CoreSTM32Hal
	CoreEventFlags
	FileManagerKit
	VideoKit
)

target_link_custom_leka_targets(spike_lk_lcd_serial)
