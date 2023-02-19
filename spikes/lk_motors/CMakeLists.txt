# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_motors)

target_include_directories(spike_lk_motors
	PRIVATE
	.
)

target_sources(spike_lk_motors
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_motors
	CoreMotor
	CorePwm
)
