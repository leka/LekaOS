# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

OUTPUT_OS_VERSION="`(echo $OS_VERSION_FILE)`"
OUTPUT_FIRMWARE_VERSION="`(echo $FIRMWARE_VERSION_FILE)`"

if [ $OS_VERSION_FILE_ARE_SAME = "True" ]; then
	OUTPUT_OS_VERSION_SAME_AS_FILE="✔️"
else
	OUTPUT_OS_VERSION_SAME_AS_FILE="❌"
fi

if [ $OS_VERSION_ARE_SAME = "True" ]; then
	OUTPUT_OS_VERSION_SAME_AS_OS_VERSION_CONFIG="✔️"
else
	OUTPUT_OS_VERSION_SAME_AS_OS_VERSION_CONFIG="❌"
fi

if [ $FIRMWARE_VERSION_FILE_ARE_SAME = "True" ]; then
	OUTPUT_FIRMWARE_VERSION_SAME_AS_FILE="✔️"
else
	OUTPUT_FIRMWARE_VERSION_SAME_AS_FILE="❌"
fi

if [ $FIRMWARE_VERSION_ARE_SAME = "True" ]; then
	OUTPUT_FIRMWARE_VERSION_SAME_AS_OS_VERSION_CONFIG="✔️"
else
	OUTPUT_FIRMWARE_VERSION_SAME_AS_OS_VERSION_CONFIG="❌"
fi

#
# MARK: - markdown output
#

echo "Creating markdown output"

echo 'VERSION_COMPARISON_OUTPUT<<EOF_VERSION_COMPARISON_OUTPUT' >> $GITHUB_ENV

echo -n "|                                   |           Version          | Same as filename |  Same as os_version  |\n" >> $GITHUB_ENV
echo -n "|:---------------------------------:|:--------------------------:|:----------------:|:--------------------:|\n" >> $GITHUB_ENV
echo -n "|               **os**              |\`$OUTPUT_OS_VERSION\`      |$OUTPUT_OS_VERSION_SAME_AS_FILE      |$OUTPUT_OS_VERSION_SAME_AS_OS_VERSION_CONFIG      |\n" >> $GITHUB_ENV
echo -n "| **firmware**<br>(bootloader + os) |\`$OUTPUT_FIRMWARE_VERSION\`|$OUTPUT_FIRMWARE_VERSION_SAME_AS_FILE|$OUTPUT_FIRMWARE_VERSION_SAME_AS_OS_VERSION_CONFIG|\n" >> $GITHUB_ENV


echo 'EOF_VERSION_COMPARISON_OUTPUT' >> $GITHUB_ENV
