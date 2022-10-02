#!/usr/bin/env zsh

# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Setup helpers
#

setopt +o nomatch # do not error on pattern not found

SCRIPT_DIR=$(dirname $0)

function try {
	. $SCRIPT_DIR'/helpers/try.sh' $@
}

#
# MARK: - Variables
#

CURRENT_DIR=$(pwd)
INPUT_DIR="$CURRENT_DIR/fs/home/img/raw" # by default, use the raw directory
JPG_OUTPUT_DIR="$CURRENT_DIR/fs/home/img"

RESIZE_VALUE=800x480
MUST_RESIZE=false
MULTIPLE_QUALITY=false

#
# MARK: - Deal with args
#

HELP_TEXT="Usage: $0 -i [DIRECTORY] [-q] [-r FORMAT]

	-i	{directory} input directory of PNG files to convert
	-r	{format} resize, format must be WidthxHeight (e.g. 800x480)
	-q	Set multiple JPG quality from 100 to 91. False by default with quality at 100
	-h	Show this message
"

while getopts hi:qr: flag; do
	case ${flag} in
		h) echo "${HELP_TEXT}"
		   exit 0;;
		i) INPUT_DIR="$OPTARG";;
		r) RESIZE_VALUE=$OPTARG;;
		q) MULTIPLE_QUALITY=true;;
	esac
done

if [[ $(command -v convert) == "" ]]; then
   echo "convert is not installed, install with:"
   echo "\n\tbrew install imagemagick\n"
   echo "${HELP_TEXT}"
   exit 1
fi

if [ -z "$INPUT_DIR" ]; then
	echo "Input directory is mandatory"
	echo "${HELP_TEXT}"
	exit 1
fi

if [ ! -d "$INPUT_DIR" ]; then
	echo "$INPUT_DIR DOES NOT exist \n"
	echo "${HELP_TEXT}"
	exit 1
fi

FILE_COUNT=$(ls -1 $INPUT_DIR/*.png 2>/dev/null | wc -l)
if [ "$FILE_COUNT" == "0" ]; then
	echo "No .png files available in $INPUT_DIR\n"
	echo "${HELP_TEXT}"
	exit 1
fi

if [ -z "$RESIZE_VALUE" ]; then
	MUST_RESIZE=false
fi

#
# MARK: - Functions
#

function convertDefault() {
	try convert $1                         \
		-define jpeg:optimize-coding=false \
		-quality 100                       \
		-sampling-factor 4:4:4             \
		-colorspace RGB                    \
		$JPG_OUTPUT_DIR/$2.jpg
}

function convertMultipleQuality() {
	for quality in {100..91}; do
		try convert $1                         \
			-define jpeg:optimize-coding=false \
			-quality $quality                  \
			-sampling-factor 4:4:4             \
			-colorspace RGB                    \
			$JPG_OUTPUT_DIR/$2---$quality.jpg
	done
}

function convertAndResize() {
	try convert $1                         \
		-resize $RESIZE_VALUE              \
		-define jpeg:optimize-coding=false \
		-quality 100                       \
		-sampling-factor 4:4:4             \
		-colorspace RGB                    \
		$JPG_OUTPUT_DIR/$2.jpg
}

function convertMultipleQualityAndResize() {
	for quality in {100..91}; do
		try convert $1                         \
			-resize $RESIZE_VALUE              \
			-define jpeg:optimize-coding=false \
			-quality $quality                  \
			-sampling-factor 4:4:4             \
			-colorspace RGB                    \
			$JPG_OUTPUT_DIR/$2---$quality.jpg
	done
}

#
# MARK: - Main script
#

mkdir -p $JPG_OUTPUT_DIR

for file in "$INPUT_DIR"/*.png; do

	filename=$file:t:r

	if [ "$MULTIPLE_QUALITY" = true ] && [ "$MUST_RESIZE" = true ]; then
		convertMultipleQualityAndResize $file $filename
	elif [ "$MULTIPLE_QUALITY" = true ]; then
		convertMultipleQuality $file $filename
	elif [ "$resize" = true ]; then
		convertAndResize $file $filename
	else
		convertDefault $file $filename
	fi

done

list_failed_commands
