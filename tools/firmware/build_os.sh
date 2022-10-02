#!/bin/bash

mkdir -p _tmp
mkdir -p _release

# Variables
RECOMPILE_APPLICATIONS="$1"

APPLICATION_HEX_SOURCE="$2"
APPLICATION_VERSION="$3"
APPLICATION_HEX="_tmp/application.hex"
APPLICATION_SIGNED_HEX="$4"
APPLICATION_SIGNED_BIN="_release/application-signed.bin"

if [ -z "$APPLICATION_HEX_SOURCE" ]; then
	echo "APPLICATION_HEX_SOURCE is unset"
	APPLICATION_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/os/LekaOS.hex"
fi

if [ -z "$APPLICATION_VERSION" ]; then
	echo "APPLICATION_VERSION is unset"
	APPLICATION_VERSION="1.2.3+4"
fi

if [ -z "$APPLICATION_SIGNED_HEX" ]; then
	echo "APPLICATION_SIGNED_HEX is unset"
	APPLICATION_SIGNED_HEX="_tmp/application-signed.hex"
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
