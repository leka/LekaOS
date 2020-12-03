# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_audio STATIC)

target_include_directories(driver_audio
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(driver_audio
  PRIVATE
    source/AudioFrequency.cpp
    source/AudioPlayer.cpp
    source/WaveAudioStream.cpp
)

target_link_libraries(driver_audio mbed-os)
