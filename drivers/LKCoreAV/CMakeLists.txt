# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_LKCoreAV STATIC)

target_include_directories(driver_LKCoreAV
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
  ${CMAKE_CURRENT_SOURCE_DIR}/include/internal/jpeg
)

target_sources(driver_LKCoreAV
  PRIVATE
    source/st_otm8009a.c
    source/dsi.cpp
    source/LKScreen.cpp
    source/st_sdram.c
    source/LKCoreAV.cpp
    source/ltdc.cpp
    source/jpeg/st_jpeg_utils.cpp
    source/jpeg/st_decode_dma.cpp
    source/jpeg/st_decode_polling.cpp
    source/jpeg/st_decode_interrupt.cpp
    source/jpeg/jpeg.cpp
    source/dma2d.cpp
)

target_link_libraries(driver_LKCoreAV mbed-os)
