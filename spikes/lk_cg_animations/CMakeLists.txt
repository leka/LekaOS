# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_cg_animations)

target_sources(spike_lk_cg_animations
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_cg_animations
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_cg_animations
		lib_LekaScreen
		lib_HelloWorld
		LKCoreFatFs
		LKCoreVideo
		LKCoreLL
		LKCoreSTM32Hal
		LKAnimationKit
)

target_link_custom_leka_targets(spike_lk_cg_animations)
