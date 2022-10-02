# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreFlashMemory STATIC)

target_include_directories(CoreFlashMemory
	PUBLIC
	include
)

target_sources(CoreFlashMemory
	PRIVATE
	source/CoreFlashIS25LP016D.cpp
	source/CoreFlashManagerIS25LP016D.cpp
)

target_link_libraries(CoreFlashMemory
	CoreQSPI
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreFlashIS25LP016D_test.cpp
		tests/CoreFlashManagerIS25LP016D_test.cpp
	)
endif()
