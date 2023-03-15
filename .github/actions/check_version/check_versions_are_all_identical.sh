# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

if [ $OS_VERSION_FILE_ARE_SAME != "True" ]; then
	exit 1
fi

if [ $OS_VERSION_ARE_SAME != "True" ]; then
	exit 1
fi

if [ $FIRMWARE_VERSION_FILE_ARE_SAME != "True" ]; then
	exit 1
fi

if [ $FIRMWARE_VERSION_ARE_SAME != "True" ]; then
	exit 1
fi

exit 0
