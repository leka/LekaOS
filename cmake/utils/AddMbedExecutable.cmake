
# Note: some code originally from here: https://os.mbed.com/cookbook/mbed-cmake

# needed for memory map script
check_python_package(intelhex HAVE_INTELHEX)
check_python_package(prettytable HAVE_PRETTYTABLE)

set(CAN_RUN_MEMAP TRUE)
if(NOT (HAVE_INTELHEX AND HAVE_PRETTYTABLE))
	message(WARNING "Unable to run the memory mapper due to missing Python dependencies.")
	set(CAN_RUN_MEMAP FALSE)
endif()

# Set memap toolchain name
set(MEMAP_TOOLCHAIN_NAME ${MBED_TOOLCHAIN_NAME})


# figure out objcopy command
get_filename_component(TOOLCHAIN_BIN_DIR ${CMAKE_C_COMPILER} DIRECTORY)
set(OBJCOPY_NAME arm-none-eabi-objcopy)

find_program(
	OBJCOPY_EXECUTABLE
	NAMES ${OBJCOPY_NAME}
	HINTS ${TOOLCHAIN_BIN_DIR}
	DOC "Path to objcopy executable")

if(NOT EXISTS ${OBJCOPY_EXECUTABLE})
	message(FATAL_ERROR "Failed to find objcopy executable!  Please set OBJCOPY_EXECUTABLE to the path to ${OBJCOPY_NAME}")
endif()

#custom function to add a mbed executable and generate mbed source files
function(add_mbed_executable EXECUTABLE)

	add_executable(${EXECUTABLE} ${ARGN})

	target_link_libraries(${EXECUTABLE} mbed-os)

	set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.bin)
	set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.hex)

	set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map)

	# add link options to generate memory map
	target_link_libraries(${EXECUTABLE} -Wl,\"-Map=${MAP_FILE}\",--cref)

	set(OBJCOPY_ELF_TO_BIN_COMMAND ${OBJCOPY_EXECUTABLE} -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE})
	set(OBJCOPY_ELF_TO_HEX_COMMAND ${OBJCOPY_EXECUTABLE} -O ihex   $<TARGET_FILE:${EXECUTABLE}> ${HEX_FILE})

	# generate .bin firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND ${OBJCOPY_ELF_TO_BIN_COMMAND}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${BIN_FILE}")

	# generate .hex firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND ${OBJCOPY_ELF_TO_HEX_COMMAND}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${HEX_FILE}")

	set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${BIN_FILE} ${MAP_FILE} ${HEX_FILE})

	if(CAN_RUN_MEMAP)
		add_custom_command(
			TARGET ${EXECUTABLE} POST_BUILD
			COMMAND ${Python3_EXECUTABLE} ${MBED_OS_DIR}/tools/memap.py -t ${MEMAP_TOOLCHAIN_NAME} ${MAP_FILE}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Displaying memory map for ${EXECUTABLE}")
	endif()

endfunction(add_mbed_executable)
