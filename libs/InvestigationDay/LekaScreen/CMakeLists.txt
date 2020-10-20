# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaScreen STATIC)

target_include_directories(lib_LekaScreen
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(lib_LekaScreen
  PRIVATE
  	source/LekaScreen.cpp
	include/internal/jpeg_utils.cpp
	include/internal/decode_polling.cpp
	include/internal/stm32f7xx_hal_msp.cpp
)

target_link_libraries(lib_LekaScreen mbed-os)
