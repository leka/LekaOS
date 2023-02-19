# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_bluetooth)

target_include_directories(spike_lk_bluetooth
	PRIVATE
	.
)

target_sources(spike_lk_bluetooth
	PRIVATE
	BM64Converter.cpp
	LekaBluetooth.cpp
	main.cpp
)
