# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreLL STATIC)

target_include_directories(LKCoreLL
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(LKCoreLL
  PRIVATE
    source/LKCoreLL.cpp
)

target_link_libraries(LKCoreLL mbed-os)
