# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_vibrations)

target_include_directories(spike_lk_vibrations
	PRIVATE
		.
)

target_sources(spike_lk_vibrations
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_vibrations
	LKCoreSTM32Hal
	CoreVibration
)

target_link_custom_leka_targets(spike_lk_vibrations)