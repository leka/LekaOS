include(CMakeParseArguments)

# converts a list into a string with each of its elements seperated by a space
macro(list_to_space_separated OUTPUT_VAR) # 2nd arg: LIST...
	string(REPLACE ";" " " ${OUTPUT_VAR} "${ARGN}")
endmacro(list_to_space_separated)

# causes a symlink between FILE and SYMLINK to be created at install time.
# the paths of FILE and SYMLINK are appended to the install prefix
# only works on UNIX
macro(installtime_create_symlink FILE SYMLINK) # 3rd optional arg: COMPONENT
	# cmake -E create_symlink doesn't work on non-UNIX OS's
	if(HOST_WINDOWS)
		message(FATAL_ERROR "installtime_create_symlink called on a non-UNIX platform")
	endif()

	if("${ARGN}" STREQUAL "")
		install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${FILE} \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${SYMLINK})")
	else()
		install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${FILE} \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${SYMLINK})" COMPONENT ${ARGN})
	endif()
endmacro(installtime_create_symlink)

# creates a rule to make OUTPUTFILE from the output of running m4 on INPUTFILE
macro(m4_target INPUTFILE OUTPUTFILE) # 3rd arg: M4_OPTIONS
	add_custom_command(
		OUTPUT ${OUTPUTFILE}
		COMMAND m4 ${ARGN}
		ARGS ${INPUTFILE} > ${OUTPUTFILE} VERBATIM)
endmacro(m4_target)

# Checks that the cache variable VARIABLE is set to one of VALID_VALUES and prints an error if is not.
# Also creates a pull-down menu for the variable in the GUI containing these choices
macro(validate_configuration_enum VARIABLE) # 2nd argument: VALID_VALUES...
	list_contains(VALID ${${VARIABLE}} ${ARGN})

	if(NOT VALID)
		list_to_space_separated(VALID_VALUES_STRING ${ARGN})

		message(FATAL_ERROR "${${VARIABLE}} is not a valid value for ${VARIABLE} -- must be one of: ${VALID_VALUES_STRING}")
	endif()

	set_property(CACHE ${VARIABLE} PROPERTY STRINGS ${ARGN})
endmacro(validate_configuration_enum)

# Remove the last file extension from a filename.
# foo.bar.s > foo.bar
# This is different from get_filename_component(.. NAME_WE), where foo.bar.s > foo
macro(strip_last_extension OUTPUT_VAR FILENAME)
	# from http://stackoverflow.com/questions/30049180/strip-filename-shortest-extension-by-cmake-get-filename-removing-the-last-ext
	string(REGEX REPLACE "\\.[^.]*$" "" ${OUTPUT_VAR} ${FILENAME})
endmacro()

# several times in this codebase we have a library and an executable named the same thing.
# in that case, we use a "lib" prefix on the target to distinguish the two.
# normally, that would mean we'd get a library file like "liblibsander.so"

# this macro removes the lib prefix on each of the library targets provided so that this doesn't happen
macro(remove_prefix) # LIBRARIES
	set_target_properties(${ARGN} PROPERTIES PREFIX "")
	set_target_properties(${ARGN} PROPERTIES IMPORT_PREFIX "")
endmacro(remove_prefix)

# make the provided object library position independent if shared libraries are turned on
function(make_pic_if_needed OBJECT_LIBRARY)
	set_property(TARGET ${OBJECT_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ${SHARED})
endfunction(make_pic_if_needed)

# Append NEW_FLAGS to the COMPILE_FLAGS property of each source file in SOURCE
macro(append_compile_flags NEW_FLAGS) # SOURCE...
	foreach(SOURCE_FILE ${ARGN})
		get_property(CURRENT_COMPILE_FLAGS SOURCE ${SOURCE_FILE} PROPERTY COMPILE_FLAGS)

		set(NEW_COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} ${NEW_FLAGS}")

		set_property(SOURCE ${SOURCE_FILE} PROPERTY COMPILE_FLAGS ${NEW_COMPILE_FLAGS})
	endforeach()
endmacro(append_compile_flags)

# removes the given directory from cmake's PATH environment variable
function(remove_from_path DIRECTORY)
	if(HOST_WINDOWS)
		set(CONVERTED_PATH $ENV{PATH})
	else()
		string(REPLACE ":" ";" CONVERTED_PATH $ENV{PATH})
	endif()

	get_filename_component(TO_REMOVE_REALPATH "${DIRECTORY}" REALPATH)

	set(NEW_PATH "")

	foreach(PATHCOMPONENT ${CONVERTED_PATH})
		get_filename_component(PATHCOMPONENT_REALPATH "${PATHCOMPONENT}" REALPATH)

		# message("Comparing \"${TO_REMOVE_REALPATH}\" and \"${PATHCOMPONENT_REALPATH}\"")

		# make sure to compare their real paths, so we aren't folled by any path weirdness in PATH
		if(NOT "${TO_REMOVE_REALPATH}" STREQUAL "${PATHCOMPONENT_REALPATH}")
			list(APPEND NEW_PATH "${PATHCOMPONENT}")
		endif()
	endforeach()

	# printvar(ENV{PATH})
	# printvar(NEW_PATH)

	# now set the new path
	if(HOST_WINDOWS)
		set($ENV{PATH} "${NEW_PATH}")
	else()
		string(REPLACE ";" ":" ENV{PATH} "${NEW_PATH}")
	endif()

	# printvar(ENV{PATH})
endfunction(remove_from_path)

function(target_link_custom_leka_targets TARGET)
	# foreach(board IN ITEMS ${AVAILABLE_CUSTOM_TARGETS})
	# if (${TARGET_BOARD} MATCHES ${board})
	# message(STATUS "Adding ${board} library dependency to ${TARGET}")
	# target_link_libraries(${TARGET} target_${board})
	# endif()
	# endforeach()
endfunction()

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
