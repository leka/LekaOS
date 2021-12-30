# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-leka-disco INTERFACE)

target_include_directories(mbed-leka-disco
	INTERFACE
	.
)

target_sources(mbed-leka-disco
	INTERFACE
		PeripheralPins.c
		system_clock.c
)

target_link_libraries(mbed-leka-disco
	INTERFACE
		mbed-stm32f769xi
)
