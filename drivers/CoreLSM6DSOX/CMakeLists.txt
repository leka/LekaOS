# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

<<<<<<< HEAD
add_library(CoreLSM6DSOX STATIC)

target_include_directories(CoreLSM6DSOX
=======
add_library(coreLSM6DSOX STATIC)

target_include_directories(coreLSM6DSOX
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
	PUBLIC
	include
	extern/include
)

<<<<<<< HEAD
target_sources(CoreLSM6DSOX
=======
target_sources(coreLSM6DSOX
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
	PRIVATE
	source/CoreLSM6DSOX.cpp
	source/CoreAccelerometer.cpp
	source/CoreGyroscope.cpp
	extern/source/lsm6dsox_reg.c
)

<<<<<<< HEAD
target_link_libraries(CoreLSM6DSOX)
=======
target_link_libraries(coreLSM6DSOX)
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
