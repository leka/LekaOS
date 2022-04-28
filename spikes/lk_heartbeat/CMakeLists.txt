# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_heartbeat)

target_include_directories(spike_lk_heartbeat
	PRIVATE
		.
)

target_sources(spike_lk_heartbeat
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_heartbeat
	CoreLED
	ColorKit
	LedKit
	CoreEventFlags
)

target_link_custom_leka_targets(spike_lk_heartbeat)
