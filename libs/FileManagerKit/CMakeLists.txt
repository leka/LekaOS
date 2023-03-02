# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(FileManagerKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/File.cpp
		source/FileManagerKit.cpp
		source/FileReception.cpp

	PUBLIC_LINK_LIBRARIES
		mbed-storage-fat
		mbed-storage-sd

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		mbed-mbedtls
		CoreEventQueue
		ContainerKit

	UNIT_TESTS_SOURCES
		tests/File_buffer_test.cpp
		tests/File_clear_test.cpp
		tests/File_error_test.cpp
		tests/File_flush_test.cpp
		tests/File_initialization_test.cpp
		tests/File_open_close_test.cpp
		tests/File_read_test.cpp
		tests/File_reopen_test.cpp
		tests/File_seek_test.cpp
		tests/File_sha256_test.cpp
		tests/File_size_test.cpp
		tests/File_tell_test.cpp
		tests/File_write_test.cpp
		tests/FileManagerKit_test.cpp
		tests/FileReception_test.cpp
)
