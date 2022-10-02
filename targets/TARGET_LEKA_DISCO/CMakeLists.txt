# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_library(target_DISCO_ORIGINAL STATIC)

target_include_directories(target_DISCO_ORIGINAL
	PUBLIC
	.
)

target_sources(target_DISCO_ORIGINAL
	PRIVATE
	PeripheralPins.c
	system_clock.c
)

target_link_libraries(target_DISCO_ORIGINAL mbed-os)
