# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_serial_number)

target_include_directories(spike_lk_serial_number
	PRIVATE
	.
)

target_sources(spike_lk_serial_number
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_serial_number
	CoreMCU
	SerialNumberKit
)
