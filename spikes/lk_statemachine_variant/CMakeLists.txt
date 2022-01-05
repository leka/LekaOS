# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_statemachine_variant)

target_include_directories(spike_lk_statemachine_variant
	PRIVATE
		.
)

target_sources(spike_lk_statemachine_variant
	PRIVATE
		main.cpp
		StateMachine.cpp
)

target_link_custom_leka_targets(spike_lk_statemachine_variant)
