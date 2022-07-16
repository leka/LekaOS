# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_game_touch_speed)

target_include_directories(spike_lk_game_touch_speed
	PRIVATE
	.
)

target_sources(spike_lk_game_touch_speed
	PRIVATE
	main.cpp
	LedManager.cpp
	Level.cpp
	Round.cpp
)

target_link_libraries(spike_lk_game_touch_speed
	TouchSensorKit
	CoreLED
	CoreSPI
	LedKit
	EventLoopKit
	Utils
)

target_link_custom_leka_targets(spike_lk_game_touch_speed)
