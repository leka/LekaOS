# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreSTM32Hal STATIC)

target_include_directories(LKCoreSTM32Hal
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(LKCoreSTM32Hal
  PRIVATE
    source/LKCoreSTM32Hal.cpp
)

target_link_libraries(LKCoreSTM32Hal
	PUBLIC
		mbed-os-static
)
