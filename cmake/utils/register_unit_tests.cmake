# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# Define function to add sources to LekaOS Unit Tests
function(leka_unit_tests_sources UNIT_TESTS_SOURCES)
	if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
		set(_UNIT_TESTS_SOURCES ${UNIT_TESTS_SOURCES} ${ARGN})
		target_sources(LekaOSUnitTestsExec
			PRIVATE
			${_UNIT_TESTS_SOURCES}
		)
	endif()
endfunction()

# Define function to register new unit tests for libraries
function(leka_register_unit_tests_for_library LIBRARY)
	if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
		add_subdirectory("${LIBS_DIR}/${LIBRARY}" "libs/${LIBRARY}")
		target_link_libraries(LekaOSUnitTestsExec
			PRIVATE
			${LIBRARY}
		)
	endif()
endfunction()

# Define function to register new unit tests for drivers
function(leka_register_unit_tests_for_driver DRIVER)
	if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
		add_subdirectory("${DRIVERS_DIR}/${DRIVER}" "drivers/${DRIVER}")
		target_link_libraries(LekaOSUnitTestsExec
			PRIVATE
			${DRIVER}
		)
	endif()
endfunction()
