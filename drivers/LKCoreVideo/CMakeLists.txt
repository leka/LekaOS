# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreVideo STATIC)

target_include_directories(LKCoreVideo
	PUBLIC
		include
		include/internal
		include/internal/jpeg
)

target_sources(LKCoreVideo
  PRIVATE
	source/LKCoreDMA2D.cpp
	source/LKCoreDSI.cpp
	source/LKCoreJPEG.cpp
	source/LKCoreLTDC.cpp
	source/st_jpeg_utils.cpp
	source/st_otm8009a.cpp
	source/st_sdram.cpp
    source/LKCoreLCD.cpp
    source/LKCoreVideo.cpp
)

target_link_libraries(LKCoreVideo
	PRIVATE
		mbed-os
		LKCoreSTM32Hal
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreDMA2D_test.cpp
	)

endif()
