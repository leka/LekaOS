
# include correct mbed-cmake buildfile based on toolchain
if(MBED_UNITTESTS)
	include(MbedUnitTests)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	include(ToolchainArmClangBuild)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(ToolchainGCCArmBuild)
endif()
