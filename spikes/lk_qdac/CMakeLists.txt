# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_qdac)

target_include_directories(spike_lk_qdac
	PRIVATE
	.
)

target_sources(spike_lk_qdac
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_qdac
	CoreI2C
	CoreQDAC
)

target_link_custom_leka_targets(spike_lk_qdac)
