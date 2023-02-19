# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_sensors_microphone)

target_include_directories(spike_lk_sensors_microphone
	PRIVATE
	.
)

target_sources(spike_lk_sensors_microphone
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_sensors_microphone
	CoreMicrophone
)
