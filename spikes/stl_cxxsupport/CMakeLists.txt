# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_stl_cxxsupport)

target_include_directories(spike_stl_cxxsupport
	PRIVATE
	.
)

target_sources(spike_stl_cxxsupport
	PRIVATE
	main.cpp
)
