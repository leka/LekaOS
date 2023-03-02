# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

include(register_unit_tests)

# Add core+driver library target
function(add_lib_core target_name)
	set(options "")
	set(oneValueArgs "")
	set(multiValueArgs PUBLIC_INCLUDE_DIRS PRIVATE_INCLUDE_DIRS PUBLIC_SOURCES PRIVATE_SOURCES PUBLIC_LINK_LIBRARIES PRIVATE_LINK_LIBRARIES UNIT_TESTS_SOURCES)

	cmake_parse_arguments(ADD_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	_add_lib(
		TARGET
			${target_name}
		PUBLIC_INCLUDE_DIRS
			${ADD_LIB_PUBLIC_INCLUDE_DIRS}
		PRIVATE_INCLUDE_DIRS
			${ADD_LIB_PRIVATE_INCLUDE_DIRS}
		PUBLIC_SOURCES
			${ADD_LIB_PUBLIC_SOURCES}
		PRIVATE_SOURCES
			${ADD_LIB_PRIVATE_SOURCES}
		PUBLIC_LINK_LIBRARIES
			${ADD_LIB_PUBLIC_LINK_LIBRARIES}
		PRIVATE_LINK_LIBRARIES
			${ADD_LIB_PRIVATE_LINK_LIBRARIES}
		UNIT_TESTS_SOURCES
			${ADD_LIB_UNIT_TESTS_SOURCES}
	)

	leka_register_unit_tests_for_driver(${target_name})
endfunction(add_lib_core)

# Add kit library target
function(add_lib_kit target_name)
	set(options "")
	set(oneValueArgs "")
	set(multiValueArgs PUBLIC_INCLUDE_DIRS PRIVATE_INCLUDE_DIRS PUBLIC_SOURCES PRIVATE_SOURCES PUBLIC_LINK_LIBRARIES PRIVATE_LINK_LIBRARIES UNIT_TESTS_SOURCES)

	cmake_parse_arguments(ADD_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	_add_lib(
		TARGET
			${target_name}
		PUBLIC_INCLUDE_DIRS
			${ADD_LIB_PUBLIC_INCLUDE_DIRS}
		PRIVATE_INCLUDE_DIRS
			${ADD_LIB_PRIVATE_INCLUDE_DIRS}
		PUBLIC_SOURCES
			${ADD_LIB_PUBLIC_SOURCES}
		PRIVATE_SOURCES
			${ADD_LIB_PRIVATE_SOURCES}
		PUBLIC_LINK_LIBRARIES
			${ADD_LIB_PUBLIC_LINK_LIBRARIES}
		PRIVATE_LINK_LIBRARIES
			${ADD_LIB_PRIVATE_LINK_LIBRARIES}
		UNIT_TESTS_SOURCES
			${ADD_LIB_UNIT_TESTS_SOURCES}
	)

	leka_register_unit_tests_for_library(${target_name})
endfunction(add_lib_kit)

# private - add_library target
function(_add_lib)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs PUBLIC_INCLUDE_DIRS PRIVATE_INCLUDE_DIRS PUBLIC_SOURCES PRIVATE_SOURCES PUBLIC_LINK_LIBRARIES PRIVATE_LINK_LIBRARIES UNIT_TESTS_SOURCES)

	cmake_parse_arguments(_ADD_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_library(${_ADD_LIB_TARGET} STATIC)

	target_include_directories(${_ADD_LIB_TARGET}
		PUBLIC
			${_ADD_LIB_PUBLIC_INCLUDE_DIRS}
		PRIVATE
			${_ADD_LIB_PRIVATE_INCLUDE_DIRS}
	)

	target_sources(${_ADD_LIB_TARGET}
		PUBLIC
			${_ADD_LIB_PUBLIC_SOURCES}
		PRIVATE
			${_ADD_LIB_PRIVATE_SOURCES}
	)

	target_link_libraries(${_ADD_LIB_TARGET}
		PUBLIC
			${_ADD_LIB_PUBLIC_LINK_LIBRARIES}
		PRIVATE
			${_ADD_LIB_PRIVATE_LINK_LIBRARIES}
	)

	if(DEFINED _ADD_LIB_UNIT_TESTS_SOURCES)
		leka_unit_tests_sources(
			${_ADD_LIB_UNIT_TESTS_SOURCES}
		)
	endif()
endfunction()
