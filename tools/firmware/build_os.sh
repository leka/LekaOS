#!/bin/bash

mkdir -p _tmp
mkdir -p _release

# Variables
RECOMPILE_APPLICATIONS="$1"

APPLICATION_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/os/LekaOS.hex"
APPLICATION_HEX_DESTINATION="$2"

APPLICATION_VERSION="$3"
APPLICATION_HEX="_tmp/application.hex"
APPLICATION_SIGNED_HEX=$APPLICATION_HEX_DESTINATION
APPLICATION_SIGNED_BIN="_release/application-signed.bin"

if [ -z "$APPLICATION_VERSION" ]; then
	echo "APPLICATION_VERSION is unset"
	exit 1
fi
epoch=$(date +%s)
APPLICATION_VERSION=$APPLICATION_VERSION+$epoch

if [ -z "$APPLICATION_HEX_DESTINATION" ]; then
	echo "APPLICATION_HEX_DESTINATION is unset"
	exit 1
fi

# Compile applications
if [ "$RECOMPILE_APPLICATIONS" = "true" ];
then
	make deep_clean
	make config BUILD_TARGETS_TO_USE_WITH_BOOTLOADER=ON
fi;

make

# Get application binary
cp $APPLICATION_HEX_SOURCE $APPLICATION_HEX

# Sign application with private key
imgtool sign -k signing-keys.pem --align 4 -v $APPLICATION_VERSION --header-size 4096 --pad-header -S 0x180000 $APPLICATION_HEX $APPLICATION_SIGNED_HEX

# Convert in binary
arm-none-eabi-objcopy -I ihex -O binary $APPLICATION_SIGNED_HEX $APPLICATION_SIGNED_BIN
