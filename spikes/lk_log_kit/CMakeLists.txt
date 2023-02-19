# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_log_kit)

target_include_directories(spike_lk_log_kit
	PRIVATE
	.
)

target_sources(spike_lk_log_kit
	PRIVATE
	main.cpp
	source/utils.cpp
)

target_link_libraries(spike_lk_log_kit
	CoreEventQueue
)
