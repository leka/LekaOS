#!/bin/bash

mkdir -p _tmp
mkdir -p _release

# Variables
RECOMPILE_BOOTLOADER="false"
while getopts rv: flag
do
    case "${flag}" in
        r) RECOMPILE_BOOTLOADER="true";;
        v) APPLICATION_VERSION=$OPTARG;;
    esac
done

if [ -z "$APPLICATION_VERSION" ]; then
	echo "APPLICATION_VERSION is unset"
	exit 1
fi

BUILD_NUMBER=$(date +%s)
APPLICATION_VERSION="$APPLICATION_VERSION+$BUILD_NUMBER"

BOOTLOADER_HEX="_tmp/bootloader.hex"
APPLICATION_HEX="_tmp/application-signed.hex"

FIRMWARE_HEX="_release/firmware.hex"
FIRMWARE_BIN="_release/firmware.bin"

# Get bootloader
if [ "$RECOMPILE_BOOTLOADER" = "true" ];
then
	echo "Build bootloader"
	./tools/firmware/build_bootloader.sh $BOOTLOADER_HEX
fi;

# Get application
echo "Build application"
./tools/firmware/build_os.sh $RECOMPILE_BOOTLOADER $APPLICATION_HEX $APPLICATION_VERSION

# Merge bootloader and application
echo "Merge bootloader & application"
hexmerge.py -o $FIRMWARE_HEX --no-start-addr $BOOTLOADER_HEX $APPLICATION_HEX

# Convert in binary
arm-none-eabi-objcopy -I ihex -O binary $FIRMWARE_HEX $FIRMWARE_BIN
