# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_statemachine_alternative)

target_include_directories(spike_lk_statemachine_alternative
	PRIVATE
		.
)

target_sources(spike_lk_statemachine_alternative
	PRIVATE
		main.cpp
		VariantStateMachine.cpp
		PointerStateMachine.cpp
		SingletonStateMachine.cpp
)

target_link_custom_leka_targets(spike_lk_statemachine_alternative)
