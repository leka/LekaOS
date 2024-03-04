# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_dac)

target_include_directories(spike_lk_dac
	PRIVATE
	.
)

target_sources(spike_lk_dac
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_dac
	CoreSTM32Hal
	CoreDAC
)

target_link_custom_leka_targets(spike_lk_dac)
