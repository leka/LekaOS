#!/bin/bash

# ? application_os: "application" is the term used by MCUBoot, "os" is the term used by Leka

mkdir -p _tmp
mkdir -p _release

# Variables
RECOMPILE_APPLICATION_OS="$1"

APPLICATION_OS_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/os/LekaOS.hex"
APPLICATION_OS_HEX_DESTINATION="$2"

APPLICATION_OS_VERSION="$3"
APPLICATION_OS_HEX="_tmp/LekaOS.hex"
APPLICATION_OS_SIGNED_HEX=$APPLICATION_OS_HEX_DESTINATION
APPLICATION_OS_SIGNED_BIN="_release/LekaOS-$APPLICATION_OS_VERSION.bin"

if [ -z "$APPLICATION_OS_VERSION" ]; then
	echo "APPLICATION_OS_VERSION is unset"
	exit 1
fi

if [ -z "$APPLICATION_OS_HEX_DESTINATION" ]; then
	echo "APPLICATION_OS_HEX_DESTINATION is unset"
	exit 1
fi

# Compile application_os
if [ "$RECOMPILE_APPLICATION_OS" = "true" ];
then
	make deep_clean
	make config BUILD_TARGETS_TO_USE_WITH_BOOTLOADER=ON
fi;

make

# Get application_os binary
cp $APPLICATION_OS_HEX_SOURCE $APPLICATION_OS_HEX

# Sign application_os with private key
imgtool sign -k signing-keys.pem --align 4 -v $APPLICATION_OS_VERSION --header-size 4096 --pad-header -S 0x180000 $APPLICATION_OS_HEX $APPLICATION_OS_SIGNED_HEX

# Convert in binary
arm-none-eabi-objcopy -I ihex -O binary $APPLICATION_OS_SIGNED_HEX $APPLICATION_OS_SIGNED_BIN
