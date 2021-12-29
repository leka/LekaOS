# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

function(print_compilers_flags_options_definitions)
	set(LIST_PREFIX "\n--     ")

	function(output_flags FLAGS)
		set(PROJECT_FLAGS ${${FLAGS}})
		string(REPLACE " " "${LIST_PREFIX}" PROJECT_FLAGS "${PROJECT_FLAGS}")

		if(NOT PROJECT_FLAGS STREQUAL "")
			message(STATUS "${FLAGS} (Global):${LIST_PREFIX}${PROJECT_FLAGS}")
		else()
			message(STATUS "${FLAGS} (Global): (empty)")
		endif(NOT PROJECT_FLAGS STREQUAL "")

		message(STATUS "")

		set(PROJECT_FLAGS_BUILD_TYPE ${${FLAGS}_${CMAKE_BUILD_TYPE_UPPERCASE}})
		string(REPLACE " " "${LIST_PREFIX}" PROJECT_FLAGS_BUILD_TYPE "${PROJECT_FLAGS_BUILD_TYPE}")

		# message(STATUS "${FLAGS} (For ${CMAKE_BUILD_TYPE}):${LIST_PREFIX}${PROJECT_FLAGS_BUILD_TYPE}")
		if(NOT PROJECT_FLAGS_BUILD_TYPE STREQUAL "")
			message(STATUS "${FLAGS} (${CMAKE_BUILD_TYPE}):${LIST_PREFIX}${PROJECT_FLAGS_BUILD_TYPE}")
		else()
			message(STATUS "${FLAGS} (${CMAKE_BUILD_TYPE}): (empty)")
		endif(NOT PROJECT_FLAGS_BUILD_TYPE STREQUAL "")

		message(STATUS "")
	endfunction(output_flags)

	function(output_flags_build_type FLAGS)
		set(PROJECT_FLAGS ${${FLAGS}})
		string(REPLACE " " "${LIST_PREFIX}" PROJECT_FLAGS "${PROJECT_FLAGS}")
		message(STATUS "${FLAGS} (Global):${LIST_PREFIX}${PROJECT_FLAGS}")
		message(STATUS "")
	endfunction(output_flags_build_type)

	function(output_options_definitions OPTIONS_DEFINITIONS)
		get_property(PROJECT_${OPTIONS_DEFINITIONS} DIRECTORY PROPERTY ${OPTIONS_DEFINITIONS})
		string(REPLACE ";" "${LIST_PREFIX}" PROJECT_${OPTIONS_DEFINITIONS} "${PROJECT_${OPTIONS_DEFINITIONS}}")

		if(NOT PROJECT_${OPTIONS_DEFINITIONS} STREQUAL "")
			message(STATUS "${OPTIONS_DEFINITIONS}:${LIST_PREFIX}${PROJECT_${OPTIONS_DEFINITIONS}}")
		else()
			message(STATUS "${OPTIONS_DEFINITIONS}: (empty)")
		endif(NOT PROJECT_${OPTIONS_DEFINITIONS} STREQUAL "")

		message(STATUS "")
	endfunction(output_options_definitions FLAGS_OPTIONS_DEFINITIONS)

	string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPERCASE)

	message(STATUS "")
	message(STATUS ">> ")
	message(STATUS ">> Build type & compilers")
	message(STATUS ">> ")
	message(STATUS "")

	message(STATUS "CMAKE_BUILD_TYPE:   ${CMAKE_BUILD_TYPE}")
	message(STATUS "CMAKE_C_COMPILER:   ${CMAKE_C_COMPILER}")
	message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

	message(STATUS "")
	message(STATUS ">> ")
	message(STATUS ">> C/C++ flags")
	message(STATUS ">> ")
	message(STATUS "")

	output_flags(CMAKE_C_FLAGS)

	output_flags(CMAKE_CXX_FLAGS)

	output_flags(CMAKE_EXE_LINKER_FLAGS)

	message(STATUS ">> ")
	message(STATUS ">> Compile & link options/definitions")
	message(STATUS ">> ")
	message(STATUS "")

	output_options_definitions(COMPILE_OPTIONS)
	output_options_definitions(LINK_OPTIONS)
	output_options_definitions(COMPILE_DEFINITIONS)
	output_options_definitions(LINK_DEFINITIONS)

	message(STATUS ">> ")
	message(STATUS ">> CMake options")
	message(STATUS ">> ")
	message(STATUS "")

	message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS --> ${CMAKE_EXPORT_COMPILE_COMMANDS}")
	message(STATUS "CMAKE_VERBOSE_MAKEFILE        --> ${CMAKE_VERBOSE_MAKEFILE}")

	message(STATUS "")
endfunction()
