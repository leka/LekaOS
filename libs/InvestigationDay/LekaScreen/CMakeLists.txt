# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaScreen STATIC)

target_include_directories(lib_LekaScreen
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(lib_LekaScreen
  PRIVATE
  	source/LekaScreen.cpp
    source/DrawingTools.cpp
    source/LKAVKit.cpp
)

target_link_libraries(lib_LekaScreen driver_LKCoreAV)
target_link_libraries(lib_LekaScreen mbed-os)
