# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreDAC STATIC)

target_include_directories(CoreDAC
	PUBLIC
	include
)

target_sources(CoreDAC
	PRIVATE
	source/CoreSTM32HalBasicTimer.cpp
	source/CoreDAC.cpp
)

if(NOT(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests"))
	target_sources(CoreDAC
		PUBLIC
		source/HAL_IRQHandlers.cpp
	)
endif()

target_link_libraries(CoreDAC
	mbed-os
	CoreSTM32Hal
)
