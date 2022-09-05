# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLSM6DSOX STATIC)

target_include_directories(CoreLSM6DSOX
<<<<<<< HEAD
=======
add_library(coreLSM6DSOX STATIC)

target_include_directories(coreLSM6DSOX
<<<<<<< HEAD
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 98c551c9 (:construction: [drivers] CoreLSM6DSOX after sync)
	PUBLIC
	include
	extern/include
)

target_sources(CoreLSM6DSOX
<<<<<<< HEAD
=======
target_sources(coreLSM6DSOX
<<<<<<< HEAD
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 98c551c9 (:construction: [drivers] CoreLSM6DSOX after sync)
	PRIVATE
	source/CoreLSM6DSOX.cpp
	source/CoreAccelerometer.cpp
	source/CoreGyroscope.cpp
	extern/source/lsm6dsox_reg.c
)

target_link_libraries(CoreLSM6DSOX)
<<<<<<< HEAD
=======
target_link_libraries(coreLSM6DSOX)
<<<<<<< HEAD
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 98c551c9 (:construction: [drivers] CoreLSM6DSOX after sync)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
