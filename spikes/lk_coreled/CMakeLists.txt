# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_coreled)

target_include_directories(spike_lk_coreled
	PRIVATE
	.
)

target_sources(spike_lk_coreled
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_coreled
	CoreLED
)
