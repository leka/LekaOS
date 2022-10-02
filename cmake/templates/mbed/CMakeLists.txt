
# include correct mbed-cmake buildfile based on toolchain
if("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(ToolchainGCCArmBuild)
else()
	message(FATAL_ERROR "Unknown toolchain \"${MBED_TOOLCHAIN_NAME}\"")
endif()
