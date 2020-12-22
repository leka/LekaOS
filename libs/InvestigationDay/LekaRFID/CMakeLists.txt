# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaRFID STATIC)

target_include_directories(lib_LekaRFID
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(lib_LekaRFID
  PRIVATE
  	source/LekaRFID.cpp
)

target_link_libraries(lib_LekaRFID mbed-os)
