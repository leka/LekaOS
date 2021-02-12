# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_LKCoreVideo STATIC)

target_include_directories(driver_LKCoreVideo
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(driver_LKCoreVideo
  PRIVATE
    source/LKCoreDMA2D.cpp
	source/LKCoreLTDC.cpp
	source/LKCoreDSI.cpp
    source/LKCoreLCD.cpp
    source/LKCoreVideo.cpp
    source/st_sdram.cpp
    source/st_otm8009a.cpp
)

target_link_libraries(driver_LKCoreVideo mbed-os)
