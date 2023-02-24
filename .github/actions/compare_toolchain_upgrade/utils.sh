# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - create files
#

function createMapTextFile() {
	local DIR=$1
	local TARGET_NAME=$2

	python3 ./extern/mbed-os/tools/memap.py -t GCC_ARM $DIR/$TARGET_NAME.map > $DIR/$TARGET_NAME-map.txt
}

function createSizeTextFile() {
	local DIR=$1
	local TARGET_NAME=$2

	bash ./tools/get_code_size.sh $DIR/$TARGET_NAME.elf --markdown > $DIR/$TARGET_NAME-code_size.txt
}

#
# MARK: - get flash/ram
#

function getUsedFlashSize() {
	local DIR=$1
	local TARGET_NAME=$2

	local SIZE=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*' $DIR/$TARGET_NAME-code_size.txt)
	echo $SIZE
}


function getUsedFlashSizeWithPercentage() {
	local DIR=$1
	local TARGET_NAME=$2

	local SIZE=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $DIR/$TARGET_NAME-code_size.txt)
	echo $SIZE
}

function getUsedRamSize() {
	local DIR=$1
	local TARGET_NAME=$2

	local SIZE=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*' $DIR/$TARGET_NAME-code_size.txt)
	echo $SIZE
}

function getUsedRamSizeWithPercentage() {
	local DIR=$1
	local TARGET_NAME=$2

	local SIZE=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $DIR/$TARGET_NAME-code_size.txt)
	echo $SIZE
}

#
# MARK: - calculate diff/percentage
#

function diff_flash_base_head() {
	local BASE_FLASH=$1
	local HEAD_FLASH=$2

	local DIFF=$(($HEAD_FLASH - $BASE_FLASH))
	echo $DIFF
}

function diff_flash_base_head_percentage() {
	local BASE_FLASH=$1
	local HEAD_FLASH=$2

	local PERCENTAGE="$((100 * ($HEAD_FLASH - $BASE_FLASH) / $BASE_FLASH))%"
	echo $PERCENTAGE
}

function diff_flash_percentage() {
	local BASE_FLASH=$1
	local HEAD_FLASH=$2

	local PERCENTAGE="$((100 * ($HEAD_FLASH - $BASE_FLASH) / $BASE_FLASH))%"
	echo $PERCENTAGE
}
