# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_library(target_LEKA_V1_0_DEV STATIC)

target_sources(target_LEKA_V1_0_DEV
  PRIVATE
	PeripheralPins.c
	system_clock.c
)

target_include_directories(target_LEKA_V1_0_DEV
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(target_LEKA_V1_0_DEV mbed-os)
