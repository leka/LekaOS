#!/bin/bash

# ? application_os: "application" is the term used by MCUBoot, "os" is the term used by Leka

mkdir -p _tmp
mkdir -p _release

# Variables
RECOMPILE_BOOTLOADER="false"
while getopts rv: flag
do
    case "${flag}" in
        r) RECOMPILE_BOOTLOADER="true";;
        v) APPLICATION_OS_VERSION=$OPTARG;;
    esac
done

if [ -z "$APPLICATION_OS_VERSION" ]; then
	echo "APPLICATION_OS_VERSION is unset"
	exit 1
fi

BUILD_NUMBER=$(date +%s)
APPLICATION_OS_VERSION="$APPLICATION_OS_VERSION+$BUILD_NUMBER"

BOOTLOADER_HEX="_tmp/bootloader.hex"
APPLICATION_OS_HEX="_tmp/LekaOS-$APPLICATION_OS_VERSION.hex"

FIRMWARE_HEX="_release/Firmware-$APPLICATION_OS_VERSION.hex"
FIRMWARE_BIN="_release/Firmware-$APPLICATION_OS_VERSION.bin"

# Get bootloader
if [ "$RECOMPILE_BOOTLOADER" = "true" ];
then
	echo "Build bootloader"
	./tools/firmware/build_bootloader.sh $BOOTLOADER_HEX
fi;

# Get application_os
echo "Build application_os"
./tools/firmware/build_application_os.sh $RECOMPILE_BOOTLOADER $APPLICATION_OS_HEX $APPLICATION_OS_VERSION

# Merge bootloader and application_os
echo "Merge bootloader & application_os"
hexmerge.py -o $FIRMWARE_HEX --no-start-addr $BOOTLOADER_HEX $APPLICATION_OS_HEX

# Convert in binary
arm-none-eabi-objcopy -I ihex -O binary $FIRMWARE_HEX $FIRMWARE_BIN
